/*
 * Copyright 2018 Robin Andersson <me@robinwassen.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <node.h>
#include <node_buffer.h>

#include "./forcefocus.h"

namespace bindings {
  using v8::Boolean;
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::HandleScope;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::NewStringType;

  using forcefocus::FocusWindow;

  void FocusWindowExport(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.Length() < 1) {
      Local<String> message = String::NewFromUtf8(
                                  isolate,
                                  "Expected one argument",
                                  NewStringType::kNormal)
                                  .ToLocalChecked();
      isolate->ThrowException(Exception::TypeError(message));
      return;
    }

    if (!args[0]->IsObject()) {
      Local<String> message = String::NewFromUtf8(
                                  isolate,
                                  "Expected first argument to be a window handle buffer",
                                  NewStringType::kNormal)
                                  .ToLocalChecked();
      isolate->ThrowException(Exception::TypeError(message));
      return;
    }

    auto context = isolate->GetCurrentContext();
    unsigned char *windowHandleBuffer = (unsigned char *)node::Buffer::Data(
        args[0]->ToObject(context).ToLocalChecked());

    FocusWindow(windowHandleBuffer);
  }

  void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "focusWindow", FocusWindowExport);
  }
}  // namespace bindings

NODE_MODULE(module_name, bindings::Initialize)
