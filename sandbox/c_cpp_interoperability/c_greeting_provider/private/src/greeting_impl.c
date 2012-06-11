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

#include <stdio.h>
#include <stdlib.h>

#include "greeting_impl.h"

#include <osgi/bundle_context.h>

void greeting_sayHello(greeting_t instance){
  printf("Greetings from a C Greeting service (%s)\n", instance->name);
}

void register_services()
{
  BUNDLE_CONTEXT bundleContext = NULL;
  if (bundleContext_create(&bundleContext))
  {
    fprintf(stderr, "Creating a bundle context failed.");
    return;
  }

  greeting_service_t greetingService = malloc(sizeof(*greetingService));

  if (greetingService)
  {
    greetingService->instance = malloc(sizeof(*greetingService->instance));
    if (greetingService->instance)
    {
      greetingService->instance->name = IGreetingService_NAME;
      greetingService->greeting_sayHello = greeting_sayHello;

      printf("C bundle registering Greeting service\n");
      bundleContext_registerService(bundleContext, IGreetingService_NAME, greetingService, NULL, NULL);
    }
    else
    {
      printf("error: malloc");
      exit(-1);
    }
  }
  else
  {
    printf("error: malloc");
    exit(-1);
  }
}
