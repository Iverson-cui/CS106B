/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h"
#include "soundex.h"
using namespace std;

/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded)
 *
 * WARNING: The provided code is buggy!
 *
 * Add student test cases to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */

//this function extract all the characters from a string
string lettersOnly(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
//this function is examined to function properly


//this function encode all the alphabet into codes
string encode(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        // digit is 0
        if ((toUpperCase(s[i]) == 'A') || (toUpperCase(s[i]) == 'E') || (toUpperCase(s[i]) == 'I') || (toUpperCase(s[i]) == 'O') || (toUpperCase(s[i]) == 'U') || (toUpperCase(s[i]) == 'H') || (toUpperCase(s[i]) == 'W') || (toUpperCase(s[i]) == 'Y')){
            result += '0';
        }
        else if((toUpperCase(s[i]) == 'B') || (toUpperCase(s[i]) == 'F') || (toUpperCase(s[i]) == 'P') || (toUpperCase(s[i]) == 'V')){
            result += '1';
        }
        else if((toUpperCase(s[i]) == 'C') || (toUpperCase(s[i]) == 'G') || (toUpperCase(s[i]) == 'J') || (toUpperCase(s[i]) == 'K') || (toUpperCase(s[i]) == 'Q') || (toUpperCase(s[i]) == 'S') || (toUpperCase(s[i]) == 'X') || (toUpperCase(s[i]) == 'Z')){
            result += '2';
        }
        else if((toUpperCase(s[i]) == 'D') || (toUpperCase(s[i]) == 'T')) {
            result += '3';
        }
        else if((toUpperCase(s[i]) == 'L')) {
            result += '4';
        }
        else if((toUpperCase(s[i]) == 'M') || (toUpperCase(s[i]) == 'N')){
            result += '5';
        }
        else if((toUpperCase(s[i]) == 'R')){
            result += '6';
        }

    }
    return result;
}

string coalesce(string s) {
 /*
    Vector <int> result_temp;
    for(int i = 0; i < s.length(); i++){
        result_temp.add( s[i] );
    }
*/
    for(int i = 1; i < s.length(); i++){
        if(s[i] == s[i-1]){
            s.erase(i-1,1);
            i = i-1;
        }
    }
    return s;
}
//this function is used to replace the first number with a character and remove all the 0s
string replace(string s,string name) {
    string result;
    result += toUpperCase(name[0]);
    for(int i = 1; i < s.length(); i++){
        if(s[i] != '0'){
            result += s[i];
        }
    }
    return result;
}

//this function is used to adjust the length of the string

string makelength(string s) {
    if(s.length() > 4){
        s.erase(4);//truncate the string
    }
    while(true){
        if(s.length() == 4) {
            break;
        }
        s += '0';
    }
    return s;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    // s is the name
    return makelength(replace(coalesce(encode(lettersOnly(s))),s));
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    /* TODO: Fill in the remainder of this function. */
    string name;
    cout << "Enter a name you want to search" << endl;
    getLine(cin, name);
    string namecode = soundex(name);
    cout << namecode << endl;
    Vector <string> match_name;
    for(int i = 0; i < allNames.size(); i++) {
        if( soundex(allNames[i]) == namecode ) {
            match_name.add(allNames[i]);
        }
    }
    match_name.sort();
    // match_name contains all the names that have the same code with name

    cout << match_name <<endl;

}


/* * * * * * Test Cases * * * * * */

// TODO: add your STUDENT_TEST test cases here!


STUDENT_TEST("Test extraction by my examples 1") {
    string s = "  ab cc   ";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "abcc");
}
STUDENT_TEST("test extraction by my examples 2") {
    string s = "3'234";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "");
}


STUDENT_TEST("test the encoding part eg1") {
    string s = "Curie";
    string result = "20600";
    EXPECT_EQUAL(result, encode(lettersOnly(s)));
}
STUDENT_TEST("test the encoding part eg2") {
    string s = "Iverson";
    string result = "0106205";
    EXPECT_EQUAL(result, encode(lettersOnly(s)));
}
STUDENT_TEST("test the encoding part eg3") {
    string s = "hanrahan";
    string result = "00560005";
    EXPECT_EQUAL(result, encode(lettersOnly(s)));
}

STUDENT_TEST("test the coalesce part 00560005") {
    string s = "00560005";
    string result = "05605";
    EXPECT_EQUAL(result, coalesce(s));
}
STUDENT_TEST("test the coalesce part 222025") {
    string s = "222025";
    string result = "2025";
    EXPECT_EQUAL(result, coalesce(s));
}
STUDENT_TEST("test the coalesce part 222222") {
    string s = "222222";
    string result = "2";
    EXPECT_EQUAL(result, coalesce(s));
}
/*
STUDENT_TEST("Test the replace part eg1") {
    string result = "A23451"
    EXPECT_EQUAL(result,replace("2304501","Asa"));
}

STUDENT_TEST("Test the replace part eg2") {
    string s = "0002050";
    string name = "B";
    string result = "B25";
    EXPECT_EQUAL(result,replace(s,name));
}
*/
STUDENT_TEST("Test the makelength part eg1") {
    string s = "B589614";
    string result = "B589";
    EXPECT_EQUAL(result,makelength(s));
}
STUDENT_TEST("Test the makelength part eg2") {
    string s = "A1";
    string result = "A100";
    EXPECT_EQUAL(result,makelength(s));
}
STUDENT_TEST("Test the makelength part eg3") {
    string s = "F";
    string result = "F000";
    EXPECT_EQUAL(result,makelength(s));
}


/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}
