# Copyright (c) 2015, the Fletch project authors. Please see the AUTHORS file
# for details. All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE.md file.

{
  'targets': [
    {
      'target_name': 'fletch_shared',
      'type': 'static_library',
      'sources': [
        # TODO(ahe): Add header (.h) files.
        'assert.cc',
        'bytecodes.cc',
        'connection.cc',
        'flags.cc',
        'native_process_posix.cc',
        'native_socket_macos.cc',
        'native_socket_posix.cc',
        'test_case.cc',
        'utils.cc',
      ],
      'include_dirs': [
        '../../',
      ],
    },
  ],
}