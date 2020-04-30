"""Downloads artifacts and packages them for release"""

import argparse
import glob
import json
import os
import os.path
import shutil
import subprocess
import tempfile
import fnmatch
import re

import requests

API_VERSION = "application/vnd.github.v3+json"
BASE_URL = "https://api.github.com"
OWNER = "chfoo"
REPO = "cobbletext"

class Main:
    def __init__(self, args):
        self.oauth_token = args.api_token_file.read().strip()
        self.run_id = args.run_id
        self.package_version = args.package_version
        self.artifacts_doc = None
        self.artifacts = []

    def run(self):
        self.get_artifacts_doc()
        self.download_artifacts()

        for artifact in self.artifacts:
            self.process_artifact(artifact)

    def get_default_headers(self):
        return {
            "Accept": API_VERSION,
            "Authorization": f"token {self.oauth_token}"
        }

    def check_response(self, response):
        if response.status_code != 200:
            print(response.json())
            response.raise_for_status()

    def get_artifacts_doc(self):
        os.makedirs(f"{self.run_id}", exist_ok=True)
        docs_path = os.path.join(f"{self.run_id}", "artifacts.json")

        if os.path.exists(docs_path):
            print("Artifacts list exists")

            with open(docs_path) as file:
                self.artifacts_doc = json.load(file)
            return

        print("Fetching artifact list...")
        headers = self.get_default_headers()
        response = requests.get(
            f"{BASE_URL}/repos/{OWNER}/{REPO}/actions/runs/{self.run_id}/artifacts",
            headers=headers)
        self.check_response(response)

        doc = response.json()

        with open(docs_path, "w") as file:
            json.dump(doc, file)

        self.artifacts_doc = doc

    def download_artifacts(self):
        for artifact_doc in self.artifacts_doc["artifacts"]:
            artifact = {
                "name": artifact_doc["name"],
                "url": artifact_doc["archive_download_url"]
            }

            file_path = os.path.join(f"{self.run_id}",
                f"{artifact['name']}.zip")

            if os.path.exists(file_path):
                print(f"Exists {file_path}")
                self.artifacts.append(artifact)
                continue

            print(f"Fetching {artifact['url']}")
            headers = self.get_default_headers()
            response = requests.get(artifact["url"], headers=headers)
            self.check_response(response)

            with open(f"{file_path}.partial", "wb") as file:
                for chunk in response.iter_content():
                    file.write(chunk)

            os.rename(f"{file_path}.partial", file_path)
            self.artifacts.append(artifact)

    def process_artifact(self, artifact):
        file_path = os.path.join(f"{self.run_id}", f"{artifact['name']}.zip")
        print(f"Packaging {file_path}")

        with tempfile.TemporaryDirectory(prefix="cobbletext-temp") as temp_dir:
            subprocess.check_call(["7z", "x", file_path, "-o" + temp_dir])
            self.extract_inner_archives(temp_dir)

            platform = self.artifact_name_to_platform(artifact["name"])
            package_name = f"cobbletext-{self.package_version}-{platform}"
            package_dir = os.path.join(temp_dir, package_name)

            os.makedirs(package_dir, exist_ok=True)

            self.copy_artifact_files(temp_dir, package_dir, platform)
            self.add_copyright_notices(package_dir)
            self.create_package_archive(temp_dir, package_name, platform)

    def extract_inner_archives(self, temp_dir):
        sub_file_path = os.path.join(temp_dir, "cobbletext.tar.bz2")
        subprocess.check_call(["tar",
            "--extract",
            "--file", sub_file_path,
            "--directory", temp_dir
        ])
        subprocess.check_call(["rsync", "-a",
            f"{temp_dir}/build-release/install_dir/", f"{temp_dir}/"])

        vcpkg_glob_result = glob.glob(f"{temp_dir}/build-release/bin-dependencies/vcpkg-export*.7z")

        if vcpkg_glob_result:
            subprocess.check_call(["7z",
                "x", vcpkg_glob_result[0], "-o" + temp_dir])
            extracted_vcpkg_glob_result = glob.glob(
                f"{temp_dir}/vcpkg-export*/installed/*-*/")[0]

            subprocess.check_call(["rsync", "-a",
                f"{extracted_vcpkg_glob_result}/", f"{temp_dir}/"])

        emscripten_icu_dir = f"{temp_dir}/icu_emscripten_prefixed/icu_installed/"

        if os.path.exists(emscripten_icu_dir):
            subprocess.check_call(["rsync", "-a",
                emscripten_icu_dir, f"{temp_dir}/"])

    def artifact_name_to_platform(self, name):
        table = {
            "bin-emscripten": "emscripten",
            "bin-x64-linux-dynamic-release": "linux-x64",
            "bin-x64-osx-dynamic": "macos-x64",
            "bin-x64-windows": "windows-x64",
            "bin-x86-windows": "windows-x86",
        }
        return table[name]

    def copy_artifact_files(self, temp_dir, package_path, platform):
        bin_patterns = (
            r"include/cobbletext/.+",
            r"(lib|bin)/(lib)?cobbletext.*\.(a|bc|so|dylib|dll|lib|wasm|js)",
            r"(lib|bin)/(lib)?(bz2|freetype|harfbuzz|icudt|icudata|icuuc).*\.(a|bc|so|dylib|dll|lib)",
            r"(lib|bin)/(libpng|libz|zlib).*\.(a|bc|so|dylib|dll|lib)",

        )

        for (dirpath, dirnames, filenames) in os.walk(temp_dir):
            for name in filenames:
                rel_file_name = os.path.abspath(os.path.join(dirpath, name))\
                    .replace(os.path.abspath(temp_dir) + "/", "")

                if not any(re.match(pattern, rel_file_name) for pattern in bin_patterns):
                    continue

                file_path = os.path.join(temp_dir, rel_file_name)
                destintion_path = os.path.dirname(
                    os.path.join(package_path, rel_file_name))
                # print("copy", file_path, destintion_path)

                os.makedirs(destintion_path, exist_ok=True)
                shutil.copy2(file_path, destintion_path,
                    follow_symlinks=False)

        windows_dll = os.path.join(package_path, "lib", "cobbletext.dll")
        if os.path.exists(windows_dll):
            # Match vcpkg package convention
            better_path = os.path.join(package_path, "bin", "cobbletext.dll")
            os.rename(windows_dll, better_path)

    def add_copyright_notices(self, package_path):
        this_dir = os.path.dirname(__file__)
        notice_dir = os.path.join(this_dir, "package_overlay", "notice")
        dest_dir = os.path.join(package_path, "notice")
        shutil.copytree(notice_dir, dest_dir)

        license_file_path = os.path.join(this_dir, "..", "LICENSE")
        dest_file_path = os.path.join(package_path, "notice", "cobbletext.txt")
        shutil.copy2(license_file_path, dest_file_path)

    def create_package_archive(self, temp_dir, package_name, platform):
        if "windows" in platform:
            package_archive_path = os.path.abspath(
                os.path.join(f"{self.run_id}",f"{package_name}.7z"))
            subprocess.check_call(["7z",
                "a", package_archive_path,
                package_name], cwd=temp_dir)
        else:
            package_archive_path = os.path.abspath(
                os.path.join(f"{self.run_id}", f"{package_name}.tar.bz2"))
            subprocess.check_call(["tar", "--create", "--compress",
                "--file", package_archive_path, package_name],
                cwd=temp_dir)

def main():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument("run_id")
    arg_parser.add_argument("package_version")
    arg_parser.add_argument("--api-token-file",
        type=argparse.FileType("r"), required=True)
    args = arg_parser.parse_args()

    main = Main(args)
    main.run()

if __name__ == "__main__":
    main()
