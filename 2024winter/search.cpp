// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// This function is used to trim and tidy up the string token

string cleanToken(string token)
{
    int size = token.size();
    // make sure there is at least one letter in the string
    for (int i = 0; i < size; i++) {
        if (isalpha(token[i])) {
            break;
        }
        if (i == size - 1) {
            return "";
        }
    }
    // trim the string
    // from back
    int back = 0;
    for (int i = 0; i < size; i++) {
        char cur = token[size - 1 - i];
        if (ispunct(cur)) {
            // the cur character is a punctuation.
            continue;
        }

        back = size - i ;
        break;
    }
    // from front
    int front = 0;
    for (int i = 0; i < size; i++) {
        char cur = token[i];
        if (ispunct(cur)) {
            // the cur character is a punctuation.
            continue;
        }
        front = i;
        break;
    }
    string trim = token.substr(front , back - front);
    int sizeoftrim = trim.size();
    for (int i = 0;i < sizeoftrim; i++) {
        if ( isalpha( trim[i]) ) {
            // Convert all of the alphabet to lower case.
            trim[i] = toLowerCase(trim[i]);
        }
    }
    return trim;
}

// This is used to tidy up the content of a website into a set

Set<string> gatherTokens(string text)
{
    Set<string> uniquetokens;
    Vector <string> tokens = stringSplit(text," ");
    for (string token : tokens) {
        if (cleanToken(token) != "") {
            uniquetokens.add(cleanToken(token));
        }
    }
    return uniquetokens;
}

// this is used to read web files

Vector<string> readwebFile(string filename) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);
    return lines;
}

// based on a dbfile, the map is full of pairs of string and url
//that contain the strings

int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    Vector<string> info = readwebFile(dbfile);
    for (int i = 1; i <= info.size(); i = i+2) {
        // iterate all of the pages
        Set<string> uniquetokens = gatherTokens(info[i]);
        for (string token : uniquetokens) {
            // iterate tokens in all of the pages
            if (index.containsKey(token)) {
                index[token].add(info[i - 1]);
                continue;
            }
            Set<string> url = {info[i-1]};
            index.put(token,url);
        }
    }
    return (info.size()) / 2 ;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string> single_terms = stringSplit(query," ");
    for (int i = 0; i < single_terms.size(); i++) {
        string trimmed_term = cleanToken(single_terms[i]);
        if (i == 0) {
            result = index[trimmed_term];
        }
        else {
            if (single_terms[i][0] == '+') {
                // if + return the intersection
                result = index[trimmed_term].intersect(result);
            }
            else if (single_terms[i][0] == '-' ) {
                // if - return difference
                result = result.difference(index[trimmed_term]);
            }
            else {
                // if nothing return union with
                result = result.unionWith(index[trimmed_term]);
            }
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;
    string query;
    cout<<"indexed "<< buildIndex(dbfile,index) << " pages containing " << index.size()<<" unique terms"<< endl;
    while(true) {
        cout << "Enter query sentence (RETURN/ENTER to quit): ";
        getline(cin,query);
        if (query == "") {
            return;
        }
        cout << "Found " << findQueryMatches(index,query).size() << " matching pages" << endl;
        cout << findQueryMatches(index,query) << endl;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
    Set<string> matchesGreenAndFish = findQueryMatches(index, "green +fish");
    EXPECT_EQUAL(matchesGreenAndFish.size(), 0);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken on strings eg1") {
    EXPECT_EQUAL(cleanToken("/hel23lo/"), "hel23lo");
}
STUDENT_TEST("cleanToken on strings eg2") {
    EXPECT_EQUAL(cleanToken("~!"), "");
}
STUDENT_TEST("cleanToken on strings eg3") {
    EXPECT_EQUAL(cleanToken(""), "");
}
STUDENT_TEST("buildIndex from website.txt, 32 pages, unknown unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    //EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("ispalindrome"));
    TIME_OPERATION(32,buildIndex("res/website.txt", index));
}
