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

#include <memory>
#include "AutumnDefinition.h"
#include "AutumnException.h"
#include "IBeanWrapper.h"
#include "ILibrary.h"
#include "LocalLibrary.h"
#include "DynamicLibrary.h"
#include "AutumnConfig.h"

namespace Autumn{

AutumnConfig::AutumnConfig(IResource* config)
{
	const vector<TLibrary*> *pLibs = config->getLibraries();
	for(int i=0; i<pLibs->size(); i++){
		this->processLibrary(*(*pLibs)[i]);
	}

}

void AutumnConfig::processLibrary(TLibrary& lib)
{
	ILibrary* pl;

	if( lib.Path != LocalLibraryPath){
		pl = new DynamicLibrary(lib.Name, lib.Path);
		this->Libs.push_back(pl);
	}
	else{
		pl = LocalLibrary::getInstance();
	}
	
	int i;
	for(i=0; i<lib.Beans.size(); i++){
		this->processBean(lib.Beans[i], pl);
	}

	for(i=0; i<lib.TypeNames.size(); i++){
		this->TypeNames.push_back(lib.TypeNames[i]);
	}
}

/** Deal with each bean */
void AutumnConfig::processBean(TBean& bean, ILibrary* pl)
{
	// the bean exists or not
	if( this->Beans.find(bean.Name) != this->Beans.end() ){
		throw ReduplicateEx("AutumnConfig", "processBean", 
			"Reduplicate defined bean [" + bean.Name + "]!");
	}

	// get bean wrapper maker function
	string funName = this->mangleName("bean", "create", bean.ClassName);
	WrapperMaker* pw = (WrapperMaker*)pl->getFunction(funName);
	if( pw == NULL ){
		throw NotFoundEx("AutumnConfig", "processBean", 
			"Geting WrapperMaker failed for class '" + bean.ClassName + "'.");
	}

	// get bean wrapper freer function
	funName = this->mangleName("bean", "delete", bean.ClassName);
	WrapperFreer* pd = (WrapperFreer*)pl->getFunction(funName);
	if( pd == NULL ){
		throw NotFoundEx("AutumnConfig", "processBean", 
			"Geting WrapperFreer failed for class '" + bean.ClassName + "'.");
	}
	
	// create BeanConfig
	auto_ptr<BeanConfig> pb(new BeanConfig(bean, pw, pd));
	this->Beans.insert(make_pair(bean.Name, pb.release()));
}

string AutumnConfig::mangleName(const string& objType, 
								const string&  op, const string& objName)
{
	if( objType == "bean" ){
		if( op == "create" )
			return "create_" + objName + "_Wrapper";
		else if( op == "delete" )
			return "delete_" + objName + "_Wrapper";
	}
	throw NotFoundEx("AutumnConfig", "mangleName", 
		"type[" + objType + "]->operation[" + op +"] is not found!");
}

AutumnConfig::~AutumnConfig()
{
	AutumnLog::getInstance()->debug("AutumnConfig->~AutumnConfig");
	for(int i=0; i<this->Libs.size(); i++)
		delete this->Libs[i];
	
	/*
	 * Mustn't delete LocalLibrary singleton. BeanFactory maybe be created and deleted
	 * many times, but LocalLibrary must be created at the time of the first local
	 * function registering.
	 */
	//LocalLibrary::deleteInstance();
	
	for(map<string, BeanConfig*>::iterator it=this->Beans.begin();
			it!=this->Beans.end(); it++)
		delete it->second;
}

BeanConfig* AutumnConfig::getBeanConfig(const string& name) const
{
	const map<string, BeanConfig*>::const_iterator it = this->Beans.find(name);
	if( this->Beans.end() != it )
		return it->second;
	else
		return NULL;
}

} // End namespace Autumn
