# picat_regex
regex module for Picat

This is an experimental module for handling regexes in Picat.

It is a interface to PCRE2 (https://www.pcre.org/current/doc/html/index.html) with some selected functionality.
Especially interesting are probably these pages:
- pcre2pattern: https://www.pcre.org/current/doc/html/pcre2pattern.html
- pcre2syntax: https://www.pcre.org/current/doc/html/pcre2syntax.html
- pcre2perform: https://www.pcre.org/current/doc/html/pcre2perform.html

For more information and examples regarding regexes see for example:
- https://www.rexegg.com/ 
  Here's an overview of operators and symbols: https://www.rexegg.com/regex-quickstart.html
- https://www.regular-expressions.info/


This regex module does not cover the complete PCRE2 API, far from it. Instead it containts some few predicates/functions, mainly taken from my own use cases of regular expressions from my experience as a long time Perl programmer.


## Files:
The files in this repo are:
- emu/bp_pcre2.c 

  The C function with calls to PCRE.
  
- emu/cpreds.c 

  Includes the new regex functions.
  
- emu/Makefile.linux64_pcre2

- emu/picat_utilities.h

  This is needed for Picat v3.3#3 since the current file is not correct in the defintion of `cstring_to_picat(char* ch_ptr)`. It should be `cstring_to_picat(char* ch_ptr, int n)`). This should be fixed for later versions.
  
- emu/test_regex.pi

  Some tests (`go/0`. `go2/0` .. `go9/0`) testing different ascpects of the regex module.
  
- lib/regex.pi

  The Picat predicates/functions using the interface defined in bp_pcre2.c.


In ./ there are some test programs for the regex module and some other regex related programs.

Testing the regex module (i.e. `import regex.`)

- regex_test_emu.pi

  Some othe tests of the regex module.
  
- wordle_regex.pi

  Wordle solver. It uses the wordlist wordle_small.txt
  
. regex_match_number.pi  
  
- make_regex2.pi 

  Creates regexes given a list of words, and test them using the regex module. (This is a variant of my http://hakank.org/picat/make_regex.pi).

Some other regex related programs which does not require the regex module but I thought was appropritate to include

- regex_crossword.pi

  Solving some of the regex crosswords from https://regexcrossword.com
  
- regex_generating_strings_v3.pi

  Generating the possible strings given a regex. Note that the supported regexes are quite limited, e.g. no backreferences etc.


## Compiling and testing

The regex module has been developed on a Linux Ubuntu 20.04 system. To compile it you need the C source code for Picat, available at http://picat-lang.org/download.html (named picatnn.tar.gz, e.g. picat333.tar.gz).

* Unpack the contents for the picat *.tar.gz in a directory, say picat_333_regex

* Copy the files in this repo's directories `lib` and `emu` to the corresponding directories in the created directory 

* Ensure that the pcre2-8 library is installed.

  In my Ubuntu 20.04 this is done by
  ```
  $ sudo apt update
  $ sudo apt install pcre2-8
  ```

  (The `-8` part is for the size of the characters used in the regexps.)

* cd to the directory emu and run the make
    
     ```
     $ cd emu
     $ make -f Makefile.linux64_pcre2
     ```
     
   The Picat executable picat is now created (if no errors occurred) in this directory.
   
* test that it works as expected
   ```
   $ ./picat
   Picat> import regex
   Picat> regex("(?i)(hello),?\\s+(world|picat)!?","Hello, Picat!",Capture)
   Capture = [['H',e,l,l,o,',',' ','P',i,c,a,t,'!'],['H',e,l,l,o],['P',i,c,a,t]]
   yes   
   ```
     
   Some more tests are collected in the program `regex_test.pi`.
   ```
   $ ./picat -g go test_regex.pi
   ...
   $ ./picat -g go2 test_regex.pi 
   ...
   ```


## Supported predicates/functions in the regex module

After compiling the new picat program the `import regex.` a couple of Picat predicates/functions available (see lib/regex.pi for details). Here is a short description of the module.

Here are the (type of) parameters for the functions:

* Pattern is a PCRE2 pattern. For details, see https://www.pcre.org/current/doc/html/pcre2pattern.html

  The regexes with a backslash (\s, \w, etc) must be double quoted, e.g. the regex "^(\w){3}\s+(\w)+$" must be written as
  
       "^(\\w){3}\\s+(\\w)+$"
   
  Another gotcha is that string quotes ('"') must be quotes as well, e.g.

        "^([^\"]+?\"(.+?)\"\\s+(.+?)$"

* Subject is a string. It is the string/text that we want to do some operation on with the regex.

* Capture is a list of the captures (empty if there is no capture groups). See below for more details about the captures.
  

Here are the functions/predicates:

- `regex(Pattern,Subject)`
  True of the string Subject matches the regex Pattern.

- `regex(Pattern,Subject,Capture)`
  True of the string Subject matches the regex Pattern. Also the list Capture will contain the captures of the Pattern (if any).

- `regex_compile(Pattern)`
  Compile (caches) the regex Pattern to be used with regex_match/2-3.
  
  Note: as of now, this is a global pattern so one cannot cache more than one pattern at each time. 

- `regex_match(String)`
  `regex_match(String,Capture)`

  Same as regex/2 and regex/3 respectively, except that it uses the compiled pattern from regex_compile/1.

- `regex_replace(Pattern,Replacement,Subject,Replaced)`
  `Replaced = regex_replace(Pattern,Replacement,Subject)`

   Replaces all occurrences of Pattern in the Subject with the  string Replacement. The output is the string Replaced. In Replacement one can use backreferences such as  "$1", etc.

- `regex_replace2(Subject,Pattern,Replacement) = Replaced`
  This is a variant with the string Subject is in the first position to simplify chaining of replacements. 

  Here's a simple example of categorizing characters in vowels and consonants:
  ```
  Picat> regex_replace2("picat programming","[aeiou]","V").regex_replace2("[^aeiouV ]","C")=X
  X = ['C','V','C','V','C',' ','C','C','V','C','C','V','C','C','V','C','C']
  ```

- `regex_find(Pattern,Subject,Captures)`
  `regex_find(Pattern,Subject) = Captures`

  Find the ***first*** occurrence of Pattern in Subject and outputs it in Capture.
  Note: The list Captures contains ontly the captured groups, not the "global" matched string.

- `regex_find_all(Pattern,Subject,Matches)`
  `regex_find_all(Pattern,Subject) = Matches`

  The list Matches contain all occurrences of Pattern in the string Subject.
  
  This tries to simulate Perl's 
  ```
      while($text =~ !Pattern!gsm) {
       # ...
      }
  ```
  For strings with newlines, the Pattern should start with `"(?s)"` or `"(?sm)"` for matching newlines.
  Note: The list Matches contains ontly the captured groups, not the "global" matched string.

- `regex_find_num(Pattern,Subject,Num,Matches)`

  The list Matches contains the first Num occurrences of Pattern in  the string Subject.
  Note: The list Matches contains ontly the captured groups, not the "global" matched string.


### Flags
The program bp_pcre2.c is compiled without any flags (except for regex_replace which replaces all occurrences).

The flags - that we all know and love from Perl - are the following from https://www.pcre.org/current/doc/html/pcre2pattern.html#internaloptions )
```
- (?i)  for PCRE2_CASELESS
- (?m)  for PCRE2_MULTILINE
- (?n)  for PCRE2_NO_AUTO_CAPTURE
- (?s)  for PCRE2_DOTALL
- (?x)  for PCRE2_EXTENDED
- (?xx) for PCRE2_EXTENDED_MORE
```

The flags can be combined, e.g. `(?sm)` for combining PCRE2_MULTILINE and PCRE2_DOTALL.

Unsetting is done with "-", e.g. `(?sx)` Setting and unsetting can also be done, e.g. (?im-sx).


### Captures

Except for the predicates in the regex_find* family, the first element in Captures is the string from the first matched position to the last matched position. The rest of the elements are the individual grouping  captures (if any).

For example, in the call
   ```
   Picat> regex_match_capture("^(hakank?) was a Swedish (dancer|bassist|singer|dueller)$","hakan was a Swedish bassist",Captures3),
   ```

the Capture is 

    [hakan was a Swedish bassist,hakan,bassist]
i.e. 

   Capture[1] = "hakan was a Swedish bassist"
   Capture[2] = "hakan"
   Capture[3] = "bassist"

This can be confusing since often one wants to ignore the first "global" capture. One way might be enumerate the captured fields and ignore that first capture, e. g.
   ```
   Picat> regex_match_capture("^(hakank?) was a Swedish (dancer|bassist|singer|dueller)$",
            "hakan was a Swedish bassist",[_, Who, What]),
     -> 
      Who = "hakan"
      What = "bassist"
   ```

Note: To complicate the matter, the captures in `regex_find/3` does not have the first element as the full matched string; All[1] (or Capture[1]) is there the first matched group. This might be considered a feature...


### The C functions
The Picat definitions in lib/regex.pi calls the C function written in emu/bp_pcre2.c. Here is a short description of these with the syntax how to call them from Picat:

- bp.regex(Pattern,Subject)
- bp.regex_capture(Pattern,Subject,Capture)
- bp.regex_compile(Pattern)
- bp.regex_match(Subject)
- bp.regex_match_capture(Subject,Capture)
- bp.regex_replace(Pattern,Replacement,Subject,Replaced)
- bp.regex_find_matches(Pattern,Subject,Num,Matched).
