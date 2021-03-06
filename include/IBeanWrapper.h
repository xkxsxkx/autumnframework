/*
* Copyright 2006 the original author or authors.
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*      http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef AUTUMN_IBEANWRAPPER_H
#define AUTUMN_IBEANWRAPPER_H

#include <string>
#include <vector>
#include "Basic.h"

#ifdef WIN32
	#ifdef AUTUMNFRAMEWORK_EXPORTS
		#define DLL_IM_EXPORT _declspec(dllexport)
	#else
		#define DLL_IM_EXPORT _declspec(dllimport)
	#endif
	#ifdef _USRDLL
		#define DLL_EXPORT _declspec(dllexport)
	#else
		#define DLL_EXPORT 
	#endif
#else
	#define DLL_IM_EXPORT
	#define DLL_EXPORT 
#endif

namespace Autumn{

	/** 
	 * Wrapper of bean
	 * 
	 * @version 0.7.0
	 * @since 2007-04-27
	 */

	class DLL_IM_EXPORT IBeanWrapper{
		typedef void WrapperFreer(IBeanWrapper*);
		
		/** bean name */
		std::string BeanName;

		/** pointers of parameters, using long type */
		std::vector<TPointer> Parameters;

		/** function pointer to delete wrapper */
		WrapperFreer* WrapperDeleter;

		/** 
		 * bean's property 
		 */
		bool Singleton;								// is singleton or not
		std::string InitMethod;						// init method name
		std::string DestroyMethod;					// destroy method name
		std::string DeleteMethod;					// destruct method name
		
	public:
		virtual void* getBean() const = 0;
		virtual void  setBean(void*) = 0;
		virtual void* execCreateMethod(const std::string& method, 
									void** pPrams, int num) = 0;
		virtual int   execVoidMethod(const std::string& method, 
									void** pPrams, int num)const = 0;
		virtual int   getParamTypes (const std::string& method, 
									std::string& types, int num)const = 0;

		/** 
		 * cast bean pointer to base class pointer.
		 * @param basename base class name
		 * @return base class pointer if basename is bean's base class
		 *         or return bean pointer
		 */
		virtual void* cast2Base(const std::string& basename) const = 0;
		
		std::string getConArgTypes(const std::string& ConMethod, int num)const;
		
		void   setBeanName(const std::string& s){ this->BeanName = s; }
		std::string getBeanName() const { return this->BeanName; }
		
		void   setInitMethod(const std::string& s){ this->InitMethod = s; }
		std::string getInitMethod() const { return this->InitMethod; }
		
		void   setDestroyMethod(const std::string& s){ this->DestroyMethod = s; }
		std::string getDestroyMethod() const { return this->DestroyMethod; }
		
		void   setDeleteMethod(const std::string& s){ this->DeleteMethod = s; }
		std::string getDeleteMethod() const { return this->DeleteMethod; }
		
		void setSingleton(bool s){ this->Singleton = s; }
		bool getSingleton() const { return this->Singleton; }

		void setWrapperDeleter(WrapperFreer* wd){ this->WrapperDeleter = wd; }
		WrapperFreer* getWrapperDeleter() const { return this->WrapperDeleter; }
		
		void addParameter(TPointer p){ this->Parameters.push_back(p); }
		std::vector<TPointer> getParameter() const { return this->Parameters; }

		int setBeanPropertyValue(const char* name, void* value){
			std::string method = std::string("set") + name;
			return this->execVoidMethod(method, &value, 1);
		}
		
		int getBeanPropertyType(const char* name, std::string& type)const;
		
		/** if has deleteMethod, use this to delete bean */
		bool deleteBean();
		
		/** Destructor */
		virtual ~IBeanWrapper();
	};

	/** register local function for local library */
	DLL_IM_EXPORT void registerLocalFunction(const char* name, void* f);

} // End namespace Autumn

#endif
