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

#ifndef AUTUMNGEN_UTIL_H
#define AUTUMNGEN_UTIL_H

#include <string>
#include <iostream>

using namespace std;

/** 
 * Utility
 */
class Util{
   /**
    * If the String s starts with a string literal, a comment or white spaces,
    * return the length of the literal, comment and white spaces. String
    * literals are enclosed in " or ' and comments start with /&* or //.
    */
	static int skip(const string& s);
	
public:
	/**
	 * which line the position is in s
	 * @param idx The position
	 */
	static int lineno(const string& s, int idx);

	/** return the number of white spaces at the head of s */
	static int whiteSapceLen(const string& s);

	/** the length of implementation comment at the head of s */
	static int implCommentLen(const string& s);

	/** the length of document comment at the head of s */
	static int DocCommentLen(const string& s);

	/** the length of comment at the head of s */
	static int CommentLen(const string& s);

	/** the length of pre-processor at the head of s */
	static int preProcessortLen(const string& s);

	/** the length of literal at the head of s */
	static int literalLen(const string& s);

	/** 
	 * if has irrelevant part, return a nonzero number.
	 * a irrelevant part isn't a element of C++.
	 * it may the length of white spaces, or of implementation comment, 
	 * of define and macro. Document comment is a element.
	 * it may not the whole length of the irrelevant part.
	 */
	static int irrelevantLen(const string&s);

	/** 
	 * if has un-recognised part, return a nonzero number.
	 * it may the length of using ..., or friendly ..., or extern ...,
	 * or template ... .
	 * it may not the whole length of the un-recognised part.
	 */
	static int unrecognisedLen(const string&s);

	/**
     * Matches braces and is C-aware. It skips string literals and comments.
	 * This function can't match quotes, because skip function skips quotes.
	 * @return the index of matching c2.
     */
	static int findMatching(const string& s, char c1, char c2);

	/**
	 * Performs a C-aware version of String.indexOf(String) in that it skips
	 * string literals and comments and makes sure that the target string
	 * is not embedded in a longer word.
	 */
	static int indexOf(const string& s, const string& t);

	/**
	 * Performs a C-aware version of String.indexOf(char) in that it skips
	 * string literals and comments.
	 */
	static int indexOf(const string& s, char c);
	
	/** s begins with t, and t is not embedded in a longer word. */
	static bool startWith(const string& s, const string& t);

	/** parse base name from a file path */
	static string basenameOf(const string& s);

	/** parse directory from a file path */
	static string dirOf(const string& s);

	/** is special char or not */
	static bool isSpecialChar(char c);

	/** get the last word separated by white space or special char */
	static string getLastWord(const string& s);

	/** replace comment with white space */
	static string replaceComment(const string& r);

	/** erase white spaces at head and tail */
	static string trim(const string& s);

	/** erase all white spaces in the string */
	static string trimall(const string& s);

	/** a word is primitive data type or not */
	static bool isPrimType(const string& w);

	/** a word is struct data type key or not */
	static bool isStruTypeKey(const string& w);

	/** converts a string to uppercase */
	static string toUpper(const string& s);

	/** output message */
	static void outputMessage(const string& s){
		cout << s << endl;
	}
};

#endif
