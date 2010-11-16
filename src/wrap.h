/*
  
  Fluff.js
  Copyright (c) 2010 Ivo Wetzel.
  
  All rights reserved.
  
  Fluff.js is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Fluff.js is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  Fluff.js. If not, see <http://www.gnu.org/licenses/>.
  
*/

#include <v8.h>

using namespace v8;


// Class Wrapper ---------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef WRAP_H
#define WRAP_H

class WrappedClass {
    public:
        template <class T>
        static inline T* unwrap(Persistent<Value> value) {
            return static_cast<T*>(External::Unwrap(value->ToObject()->GetInternalField(0)));
        }
        
        template <class T>
        static inline T* unwrap(Handle<Object> value) {
            return static_cast<T*>(External::Unwrap(value->ToObject()->GetInternalField(0)));
        }
        
        virtual void destroy() = 0;
        
        template <class T>
        static inline void destroy(T* cls) {
            cls->destroy();
        }
        
        static inline void collect(Persistent<Value> value, void *data) {
            WrappedClass *cls = static_cast<WrappedClass*>(data);
            WrappedClass::destroy(cls);
        }
    
    protected:     
        Persistent<Object> self;
        Persistent<ObjectTemplate> classTemplate;
        bool classTemplateCreated; 
        
        WrappedClass() {
            classTemplateCreated = false;
        }    
        
        inline bool hasTemplate() {
            return classTemplateCreated;
        }
        
        inline Persistent<ObjectTemplate> createTemplate() {
            if (!hasTemplate()) {
                Handle<ObjectTemplate> object = ObjectTemplate::New();
                object->SetInternalFieldCount(1);
                classTemplate = Persistent<ObjectTemplate>::New(object);  
                classTemplateCreated = true;
            }
            return classTemplate;
        }
        
        inline Persistent<Object> wrap() {
            HandleScope scope;
            Handle<Object> result = classTemplate->NewInstance();
            result->SetInternalField(0, External::New(this));
            self = Persistent<Object>::New(scope.Close(result));
            self.MakeWeak(this, collect);
            return self;
        }
};
#endif

