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

#include <sstream>
#include "GenException.h"
#include "Util.h"
#include "ElmtFactory.h"
#include "NamespaceElmt.h"

bool NamespaceElmt::isThisType(const string& s, int idx)
{
	string r = s.substr(idx);

	if( !Util::startWith(r, "namespace") )
		return false;

	int brace = Util::indexOf(r, '{');
	int semicolon = Util::indexOf(r, ';');

	return ( string::npos != brace && 
		( string::npos == semicolon || brace < semicolon ) );
}

IElement* NamespaceElmt::clone(const string& s, int& idx0)
{
	// s is the whole content of head file
	string rest = s.substr(idx0);

	int brace = Util::indexOf(rest, '{');
	int endbrace = Util::findMatching(rest, '{', '}');
	if( string::npos == endbrace )
		throw GenException("Can't find matching '}' for namespace.", 
				Util::lineno(s, idx0));

	NamespaceElmt* e = new NamespaceElmt;
	e->setName( Util::getLastWord(rest.substr(0, brace)) );

	int idx, ridx;
	idx = idx0 + brace + 1;
	DocCommentElmt* dc = NULL;
	while( idx < idx0 + endbrace ){
		rest = s.substr(idx);

		if( ridx = Util::irrelevantLen(rest) ){
			idx += ridx;
		}
		else{
			IElement* child = ElmtFactory::makeElmt(s, idx);
			if( child == NULL )
				idx += Util::unrecognisedLen(rest);
			else {
				e->addChild(child);

				// if child is a document comment
				if( child->getType() == IElement::DOCCOMMENT ){
					dc = (DocCommentElmt*)child;
				}
				else if( dc != NULL ) {
					child->associateComment(dc);
					dc = NULL;
				}
			}
		}
	}
	
	// modify idx0 only when successed
	idx0 = idx0 + endbrace + 1;
	return e;
}

string NamespaceElmt::genWrapperH()
{
	string scopename, ws;

	if( ! this->getSupper().empty() ){
		scopename = this->getSupper() + "::";
	}
	scopename += this->getName();

	// only generate namespace and class for wrapper
	vector<IElement*> children = this->getChildren();
	for( int i=0; i<children.size(); i++){
		IElement* e = children[i];
		if( e->getType() == IElement::NAMESAPCE ) {
			((NamespaceElmt*)e)->setSupper(scopename);
			ws += e->genWrapperH();
		}
		else if( e->getType() == IElement::CLASS )
			ws += e->genWrapperH();
	}

	// has no content to output
	if( Util::trimall(ws).empty() )
		return "";

	ostringstream os;
	os << "using namespace " << scopename << ";" << endl << endl;
	os << ws;

	return os.str();
}

string NamespaceElmt::genWrapperCPP()
{
	string ws;

	// only generate namespace and class for wrapper
	vector<IElement*> children = this->getChildren();
	for( int i=0; i<children.size(); i++){
		IElement* e = children[i];
		if( e->getType() == IElement::NAMESAPCE ||
				e->getType() == IElement::CLASS ) {
			ws += e->genWrapperCPP();
		}
	}

	return ws;
}
