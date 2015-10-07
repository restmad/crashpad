# Copyright 2014 The Crashpad Authors. All rights reserved.
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
      'target_name': 'crashpad_util',
      'type': 'static_library',
      'dependencies': [
        '../compat/compat.gyp:crashpad_compat',
        '../third_party/mini_chromium/mini_chromium.gyp:base',
      ],
      'include_dirs': [
        '..',
        '<(INTERMEDIATE_DIR)',
        # For libbase header files.
        '<(libchromiumcontent_src_dir)/',
      ],
      'sources': [
        'file/file_io.cc',
        'file/file_io.h',
        'file/file_io_posix.cc',
        'file/file_io_win.cc',
        'file/file_reader.cc',
        'file/file_reader.h',
        'file/file_seeker.cc',
        'file/file_seeker.h',
        'file/file_writer.cc',
        'file/file_writer.h',
        'file/string_file.cc',
        'file/string_file.h',
        'mac/checked_mach_address_range.h',
        'mac/launchd.h',
        'mac/launchd.mm',
        'mac/mac_util.cc',
        'mac/mac_util.h',
        'mac/service_management.cc',
        'mac/service_management.h',
        'mac/xattr.cc',
        'mac/xattr.h',
        'mach/child_port.defs',
        'mach/child_port_handshake.cc',
        'mach/child_port_handshake.h',
        'mach/child_port_server.cc',
        'mach/child_port_server.h',
        'mach/child_port_types.h',
        'mach/composite_mach_message_server.cc',
        'mach/composite_mach_message_server.h',
        'mach/exc_client_variants.cc',
        'mach/exc_client_variants.h',
        'mach/exc_server_variants.cc',
        'mach/exc_server_variants.h',
        'mach/exception_behaviors.cc',
        'mach/exception_behaviors.h',
        'mach/exception_ports.cc',
        'mach/exception_ports.h',
        'mach/exception_types.cc',
        'mach/exception_types.h',
        'mach/mach_extensions.cc',
        'mach/mach_extensions.h',
        'mach/mach_message.cc',
        'mach/mach_message.h',
        'mach/mach_message_server.cc',
        'mach/mach_message_server.h',
        'mach/notify_server.cc',
        'mach/notify_server.h',
        'mach/scoped_task_suspend.cc',
        'mach/scoped_task_suspend.h',
        'mach/symbolic_constants_mach.cc',
        'mach/symbolic_constants_mach.h',
        'mach/task_for_pid.cc',
        'mach/task_for_pid.h',
        'mach/task_memory.cc',
        'mach/task_memory.h',
        'misc/clock.h',
        'misc/clock_mac.cc',
        'misc/clock_posix.cc',
        'misc/clock_win.cc',
        'misc/implicit_cast.h',
        'misc/initialization_state.h',
        'misc/initialization_state_dcheck.cc',
        'misc/initialization_state_dcheck.h',
        'misc/pdb_structures.cc',
        'misc/pdb_structures.h',
        'misc/scoped_forbid_return.cc',
        'misc/scoped_forbid_return.h',
        'misc/symbolic_constants_common.h',
        'misc/tri_state.h',
        'misc/uuid.cc',
        'misc/uuid.h',
        'net/http_body.cc',
        'net/http_body.h',
        'net/http_headers.cc',
        'net/http_headers.h',
        'net/http_multipart_builder.cc',
        'net/http_multipart_builder.h',
        'net/http_transport.cc',
        'net/http_transport.h',
        'net/http_transport_mac.mm',
        'net/http_transport_win.cc',
        'numeric/checked_address_range.cc',
        'numeric/checked_address_range.h',
        'numeric/checked_range.h',
        'numeric/in_range_cast.h',
        'numeric/int128.h',
        'numeric/safe_assignment.h',
        'posix/close_multiple.cc',
        'posix/close_multiple.h',
        'posix/close_stdio.cc',
        'posix/close_stdio.h',
        'posix/drop_privileges.cc',
        'posix/drop_privileges.h',
        'posix/process_info.h',
        'posix/process_info_mac.cc',
        'posix/symbolic_constants_posix.cc',
        'posix/symbolic_constants_posix.h',
        'stdlib/cxx.h',
        'stdlib/map_insert.h',
        'stdlib/objc.h',
        'stdlib/pointer_container.h',
        'stdlib/string_number_conversion.cc',
        'stdlib/string_number_conversion.h',
        'stdlib/strlcpy.cc',
        'stdlib/strlcpy.h',
        'stdlib/strnlen.cc',
        'stdlib/strnlen.h',
        'string/split_string.cc',
        'string/split_string.h',
        'synchronization/semaphore_mac.cc',
        'synchronization/semaphore_posix.cc',
        'synchronization/semaphore_win.cc',
        'synchronization/semaphore.h',
        'thread/thread.cc',
        'thread/thread.h',
        'thread/thread_log_messages.cc',
        'thread/thread_log_messages.h',
        'thread/thread_posix.cc',
        'thread/thread_win.cc',
        'win/address_types.h',
        'win/capture_context.asm',
        'win/capture_context.h',
        'win/checked_win_address_range.h',
        'win/exception_handler_server.cc',
        'win/exception_handler_server.h',
        'win/module_version.cc',
        'win/module_version.h',
        'win/nt_internals.cc',
        'win/nt_internals.h',
        'win/ntstatus_logging.cc',
        'win/ntstatus_logging.h',
        'win/process_info.cc',
        'win/process_info.h',
        'win/process_structs.h',
        'win/registration_protocol_win.cc',
        'win/registration_protocol_win.h',
        'win/scoped_handle.cc',
        'win/scoped_handle.h',
        'win/scoped_process_suspend.cc',
        'win/scoped_process_suspend.h',
        'win/time.cc',
        'win/time.h',
        'win/xp_compat.h',
      ],
      'conditions': [
        ['OS=="mac"', {
          'conditions': [
            ['GENERATOR=="ninja"', {
              # ninja’s rules can’t deal with sources that have paths relative
              # to environment variables like SDKROOT. Copy the .defs files out
              # of SDKROOT and into a place they can be referenced without any
              # environment variables.
              'copies': [
                {
                  'destination': '<(INTERMEDIATE_DIR)/util/mach',
                  'files': [
                    '$(SDKROOT)/usr/include/mach/exc.defs',
                    '$(SDKROOT)/usr/include/mach/mach_exc.defs',
                    '$(SDKROOT)/usr/include/mach/notify.defs',
                  ],
                },
              ],
              'sources': [
                '<(INTERMEDIATE_DIR)/util/mach/exc.defs',
                '<(INTERMEDIATE_DIR)/util/mach/mach_exc.defs',
                '<(INTERMEDIATE_DIR)/util/mach/notify.defs',
              ],
            }, {  # else: GENERATOR!="ninja"
              # The Xcode generator does copies after rules, so the above trick
              # won’t work, but its rules tolerate sources with SDKROOT-relative
              # paths.
              'sources': [
                '$(SDKROOT)/usr/include/mach/exc.defs',
                '$(SDKROOT)/usr/include/mach/mach_exc.defs',
                '$(SDKROOT)/usr/include/mach/notify.defs',
              ],
            }],
          ],
          'rules': [
            {
              'rule_name': 'mig',
              'extension': 'defs',
              'inputs': [
                'mach/mig.py',
              ],
              'outputs': [
                '<(INTERMEDIATE_DIR)/util/mach/<(RULE_INPUT_ROOT)User.c',
                '<(INTERMEDIATE_DIR)/util/mach/<(RULE_INPUT_ROOT)Server.c',
                '<(INTERMEDIATE_DIR)/util/mach/<(RULE_INPUT_ROOT).h',
                '<(INTERMEDIATE_DIR)/util/mach/<(RULE_INPUT_ROOT)Server.h',
              ],
              'action': [
                'python', '<@(_inputs)', '<(RULE_INPUT_PATH)', '<@(_outputs)'
              ],
              'process_outputs_as_sources': 1,
            },
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
              '$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
              '$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
              '-lbsm',
            ],
          },
        }],
        ['OS=="win"', {
          'link_settings': {
            'libraries': [
              '-lrpcrt4.lib',
              '-lwinhttp.lib',
            ],
          },
          'conditions': [
            ['target_arch=="ia32"', {
              'msvs_settings': {
                'MASM': {
                  'UseSafeExceptionHandlers': 'true',
                },
              },
            }],
          ],
        }],
      ],
    },
  ],
}
