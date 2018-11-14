// Copyright 2018-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License. 

#ifndef _dsl_error_codes_H_
#define _dsl_error_codes_H_

#define DSL_OK                          0 // For internal use only. Please use ESP_OK instead
#define ESP_ERR_DSL_BASE                0x70000
#define ESP_ERR_DSL_INVALID_LENGTH      (ESP_ERR_DSL_BASE + 1)
#define ESP_ERR_DSL_INVALID_PARAM       (ESP_ERR_DSL_BASE + 2)
#define ESP_ERR_DSL_PARAM_OUTOFRANGE    (ESP_ERR_DSL_BASE + 3)
#define ESP_ERR_DSL_UNINITIALIZED       (ESP_ERR_DSL_BASE + 4)


#endif // _dsl_error_codes_H_