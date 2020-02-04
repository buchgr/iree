# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Build the IREE project with CMake. Designed for CI, but can be run manually.

set -x
set -e

ROOT_DIR=$(git rev-parse --show-toplevel)

CMAKE_BIN=${CMAKE_BIN:-$(which cmake)}

"$CMAKE_BIN" --version
ninja --version

cd ${ROOT_DIR?}
rm -rf build/
mkdir build && cd build
"$CMAKE_BIN" -G Ninja -DCMAKE_BUILD_TYPE=FastBuild -DIREE_BUILD_COMPILER=ON -DIREE_BUILD_TESTS=ON -DIREE_BUILD_SAMPLES=OFF -DIREE_BUILD_DEBUGGER=OFF ..
"$CMAKE_BIN" --build .