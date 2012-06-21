/**
 *Licensed to the Apache Software Foundation (ASF) under one
 *or more contributor license agreements.  See the NOTICE file
 *distributed with this work for additional information
 *regarding copyright ownership.  The ASF licenses this file
 *to you under the Apache License, Version 2.0 (the
 *"License"); you may not use this file except in compliance
 *with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing,
 *software distributed under the License is distributed on an
 *"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 *specific language governing permissions and limitations
 *under the License.
 */

#ifndef COMPLEX_SERVICE_H_
#define COMPLEX_SERVICE_H_

#include <osgi/MacroUtils.h>

#define IComplexService_ID org_nativeosgi_IComplexService_1_0
#define IComplexService_NAME OSGI_XSTR(IComplexService_ID)

typedef struct complex_data *complex_data_t;
typedef struct complex_service *complex_service_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int size;
  void* data;
} vector;

typedef struct {
  int size;
  const void* data;
} vector_const;

typedef vector *vector_t;
typedef vector_const *vector_const_t;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
#endif
struct complex_service {
  complex_data_t instance;
  char* (*complex_service_toSortedString)(complex_data_t instance, vector_const_t int_vector);
};

#endif /* COMPLEX_SERVICE_H_ */
