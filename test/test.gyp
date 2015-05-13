# Copyright 2015 The Crashpad Authors. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'includes': [
    '../build/crashpad.gypi',
  ],
  'targets': [
    {
      'target_name': 'crashpad_test',
      'type': 'static_library',
      'dependencies': [
        '../compat/compat.gyp:crashpad_compat',
        '../third_party/gtest/gtest.gyp:gtest',
        '../third_party/mini_chromium/mini_chromium.gyp:base',
        '../util/util.gyp:crashpad_util',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        'errors.cc',
        'errors.h',
        'gtest_death_check.h',
        'mac/dyld.h',
        'mac/mach_errors.cc',
        'mac/mach_errors.h',
        'mac/mach_multiprocess.cc',
        'mac/mach_multiprocess.h',
        'multiprocess.h',
        'multiprocess_exec.h',
        'multiprocess_exec_posix.cc',
        'multiprocess_exec_win.cc',
        'multiprocess_posix.cc',
        'paths.cc',
        'paths.h',
        'paths_mac.cc',
        'paths_win.cc',
        'scoped_temp_dir.cc',
        'scoped_temp_dir.h',
        'scoped_temp_dir_posix.cc',
        'scoped_temp_dir_win.cc',
      ],
      'conditions': [
        ['OS=="mac"', {
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/usr/lib/libbsm.dylib',
            ],
          },
        }],
      ],
    },
  ],
}
