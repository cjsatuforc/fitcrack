'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

package_status_text_to_code_dict = {
    0: 'ready',
    1: 'finished',
    2: 'exhausted',
    3: 'malformed',
    4: 'timeout',
    10: 'running',
    11: 'validating',
    12: 'finishing'
}
host_status_text_to_code_dict = {
    0: 'benchmark',
    1: 'normal',
    2: 'validation',
    3: 'done',
    4: 'error.'
}
statuses = {
    'ready': 0,
    'finished': 1,
    'exhausted': 2,
    'malformed': 3,
    'timeout': 4,
    'running': 10,
    'validating': 11,
    'finishing': 12
}
