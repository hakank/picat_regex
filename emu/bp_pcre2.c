/*
  Regular expressions using PCRE2 in Picat.

  Syntax:  
  bp.regex(re,string)

  Note the bp. prefix!

  Examples:
  bp.regex("^a","abcd") 
  -> ok

  bp.regex_capture("^([ab]*s)\\s*(.+?)$","abbas kaviar",Capture)
  ->  [[a,b,b,a,s,' ',k,a,v,i,a,r],[a,b,b,a,s],[k,a,v,i,a,r]]
  The first capture (Capture[1]) is the matched string from first match position to the last.

  See the Picat module regex.pi for a neater interface.

  Here are some C code snippets that has been inspiration for this:
  - https://lloydrochester.com/post/c/regex_pcre/
  - https://stackoverflow.com/questions/73800119/replace-all-matches-in-pcre2-substitute-in-c

  See ../lib/regex.pi for the Picat predicates.


  Thanks to Neng-Fa Zhou for helping me with this and the Picat C interface.

  Created by Hakan Kjellerstrand (hakank@gmail.com), http://hakank.org/
  
*/
#define PCRE2_CODE_UNIT_WIDTH 8

#include "picat.h"
#include "basic.h"
#include "picat_utilities.h"
#include <pcre2.h>
#include <string.h>

/*
  regex/2:  regex(Pattern,String)
  true if the regular expression pattern matches the string string

*/
int regex() {
   
  TERM pattern_p = picat_get_call_arg(1,2); /* Regex */
  TERM subject_p = picat_get_call_arg(2,2); /* Subject string */

  char* pattern_s = picat_string_to_cstring(pattern_p);
  char* subject_s = picat_string_to_cstring(subject_p);
   
  size_t pattern_size = strlen(pattern_s);
  size_t subject_size = strlen(subject_s);  

  int ret = PICAT_FALSE; // Return value to Picat
  
  uint32_t compile_options = 0;
  PCRE2_SIZE erroffset;
  int errcode;
  PCRE2_UCHAR8 buffer[128];
  pcre2_code* re = pcre2_compile(pattern_s, pattern_size, compile_options, &errcode, &erroffset, NULL);
  if (re == NULL) {
    pcre2_get_error_message(errcode, buffer, 120);
    fprintf(stderr,"regex: %d\t%s\n", errcode, buffer);
    
    free(pattern_s);
    free(subject_s);
    
    return ret;
  }

  // pcre2_match
  int match_options = 0;
  PCRE2_SIZE* ovector;
  uint32_t ovecsize = 1024;
  pcre2_match_data* match_data = pcre2_match_data_create(ovecsize, NULL);
  int rc = pcre2_match(re, subject_s, subject_size, 0, match_options, match_data, NULL);
  if(rc == 0) {
    fprintf(stderr,"offset vector too small: %d\n",rc);
    
  } else if(rc > 0) {

    // It's a match
    ret = PICAT_TRUE;
    
  } else if (rc < 0) {

    // No match
    ; 
  }

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);
  
  if (pattern_s != NULL) {
    free(pattern_s);
  }

  if (subject_s != NULL) {
    free(subject_s);
  }

  return ret;
  
} // regex



/*
  regex_capture/2: regex_capture(Pattern,Subject,Capture)
  True if Subject matches Pattern.

  Capture is a list of captures:
  * Capture[1] is the full captured string
  * Capture[i] i>1, is the i-1 capture

*/
int regex_capture() {
  
  TERM pattern_p = picat_get_call_arg(1,3); /* Regex */
  TERM subject_p = picat_get_call_arg(2,3); /* Subject string */
  TERM capture_p = picat_get_call_arg(3,3); /* output argument: Captures */    

  char* pattern_s = picat_string_to_cstring(pattern_p);
  /* printf("pattern_s: %s\n", pattern_s); */
  /* printf("pattern len: %ld\n", strlen(pattern_s));         */
  
  char* subject_s = picat_string_to_cstring(subject_p);
  /* printf("subject_s: %s\n", subject_s); */
  /* printf("subject len: %ld\n", strlen(subject_s));       */
  
  size_t pattern_size = strlen(pattern_s);
  size_t subject_size = strlen(subject_s);  

  // printf("pattern_s: %s pattern_size: %ld\n", pattern_s, pattern_size);
  // printf("subject_s: %s subject_size: %ld\n", subject_s, subject_size);  
  
  uint32_t compile_options = 0; 
  uint32_t match_options = 0;

  // pcre2_match_data *match_data;
  uint32_t ovecsize = 1024;

  TERM ret_list = picat_build_list();
  TERM car = picat_get_car(ret_list);
  TERM cdr = picat_get_cdr(ret_list);


  int ret = PICAT_FALSE;

  PCRE2_SIZE erroffset;
  int errcode;
  PCRE2_UCHAR8 buffer[128];
  pcre2_code *re = pcre2_compile(pattern_s, pattern_size, compile_options, &errcode, &erroffset, NULL);
  if (re == NULL) {
    pcre2_get_error_message(errcode, buffer, 120);
    fprintf(stderr,"regex_capture: %d\t%s\n", errcode, buffer);
    pcre2_code_free(re);
      
    picat_unify(cdr, picat_build_nil());
    picat_unify(ret_list, capture_p); // output

    free(pattern_s);
    free(subject_s);

    return ret;
  }

  pcre2_match_data *match_data = pcre2_match_data_create(ovecsize, NULL);
  int rc = pcre2_match(re, subject_s, subject_size, 0, match_options, match_data, NULL);

  
  if(rc == 0) {
    fprintf(stderr,"offset vector too small: %d",rc);
    
  } else if(rc > 0) {
    
    PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
    PCRE2_SIZE i;
    for(int i = 0; i < rc; i++) {
      PCRE2_SPTR start = subject_s + ovector[2*i];
      PCRE2_SIZE slen = ovector[2*i+1] - ovector[2*i];
      // printf( "%2d: %.*s\n", i, (int)slen, (char *)start );

      if (i < rc-1) {
        // All but last captures
        picat_unify(car,cstring_to_picat((char*) start, slen));         
        TERM temp = picat_build_list();
        
        picat_unify(cdr, temp);
        car = picat_get_car(temp);
        cdr = picat_get_cdr(temp);
        
      } else {
        // Last capture
        picat_unify(car,cstring_to_picat((char *)start, slen));
      }
    }

    ret = PICAT_TRUE;
    
  } else if (rc < 0) {

    // No match
    ; 
  }

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);

  picat_unify(cdr, picat_build_nil()); // add tail []
  picat_unify(ret_list, capture_p); // output

  free(pattern_s);
  free(subject_s);
  
  return ret;

} // regex_capture


/*

  Compiles a regex regex_compile(Pattern).

  Note: re is a GLOBAL variable, thus it will only hold one pattern.
  Be careful.

  To be used with regex_match/1 and regex_match_capture/2

 */

PCRE2_SIZE erroffset;
int errcode;
PCRE2_UCHAR8 buffer[128];
pcre2_code* re;
// char* pattern_s;
int regex_compile() {
   
  TERM pattern_p = picat_get_call_arg(1,1); /* Regex */
  char* pattern_s = picat_string_to_cstring(pattern_p);
  size_t pattern_size = strlen(pattern_s);
  
  uint32_t options = 0;

  // Free re from previous run
  if (re != NULL) {
    pcre2_code_free(re);
  }
  
  re = pcre2_compile(pattern_s, pattern_size, options, &errcode, &erroffset, NULL);
  if (re == NULL) {
    pcre2_get_error_message(errcode, buffer, 120);
    fprintf(stderr,"regex_compile: %d\t%s\n", errcode, buffer);
    pcre2_code_free(re);

    free(pattern_s);

    return PICAT_FALSE;
  }

  free(pattern_s);
  
  return PICAT_TRUE;

} // regex_compile



/*
  regex_match/2: regex_capture(Subject)

  Matches the previous compiled pattern.

*/
int regex_match() {

  if (re == NULL) {
    fprintf(stderr,"regex_match: No defined pattern!");
    return PICAT_FALSE;
  }
  
  TERM subject_p = picat_get_call_arg(1,1); /* Subject string */

  char* subject_s = picat_string_to_cstring(subject_p);
  // printf("subject_s: %s subject_len: %ld\n", subject_s, strlen(subject_s));
  
  size_t subject_size = strlen(subject_s);  

  uint32_t match_options = 0; 

  // pcre2_match_data *match_data;
  uint32_t ovecsize = 1024;

  int ret = PICAT_FALSE;

  pcre2_match_data *match_data = pcre2_match_data_create(ovecsize, NULL);
  int rc = pcre2_match(re, subject_s, subject_size, 0, match_options, match_data, NULL);
  
  if(rc == 0) {
    fprintf(stderr,"offset vector too small: %d",rc);
    
  } else if(rc > 0) {

    ret = PICAT_TRUE;
    
  } else if (rc < 0) {

    // No match
    ; 
  }

  pcre2_match_data_free(match_data);

  free(subject_s);
  
  return ret;

} // regex_match
 

/*
  regex_match_capture/2: regex_capture(Subject,Capture)
  Matches the previous compiled pattern.

  Capture is a list of captures:
  * Capture[1] is the full captured string
  * Capture[i] i>1, is the i-1 capture

*/
int regex_match_capture() {
  
  TERM subject_p = picat_get_call_arg(1,2); /* Subject string */
  TERM capture_p = picat_get_call_arg(2,2); /* output argument: Captures */
  
  char* subject_s = picat_string_to_cstring(subject_p);
  // printf("subject_s: %s subject_len: %ld\n", subject_s,strlen(subject_s));
  
  size_t subject_size = strlen(subject_s);  

  uint32_t match_options = 0; 
  uint32_t ovecsize = 1024;

  TERM ret_list = picat_build_list();
  TERM car = picat_get_car(ret_list);
  TERM cdr = picat_get_cdr(ret_list);

  int ret = PICAT_FALSE;

  pcre2_match_data *match_data = pcre2_match_data_create(ovecsize, NULL);
  int rc = pcre2_match(re, subject_s, subject_size, 0, match_options, match_data, NULL);
  if(rc == 0) {
    fprintf(stderr,"offset vector too small: %d",rc);
    
  } else if(rc > 0) {
    
    PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
    PCRE2_SIZE i;
    for(int i = 0; i < rc; i++) {
      PCRE2_SPTR start = subject_s + ovector[2*i];
      PCRE2_SIZE slen = ovector[2*i+1] - ovector[2*i];
      // printf( "%2d: %.*s\n", i, (int)slen, (char *)start );

      if (i < rc-1) {
        // All but last captures
        picat_unify(car,cstring_to_picat((char*) start, slen));         
        TERM temp = picat_build_list();
        
        picat_unify(cdr, temp);
        car = picat_get_car(temp);
        cdr = picat_get_cdr(temp);
        
      } else {
        // Last capture
        picat_unify(car,cstring_to_picat((char *)start, slen));
      }
    }

    ret = PICAT_TRUE;
    
  } else if (rc < 0) {

    // No match
    ; 
  }

  pcre2_match_data_free(match_data);
  // pcre2_code_free(re);
  
  picat_unify(cdr, picat_build_nil()); // add tail []
  picat_unify(ret_list, capture_p); // output

  free(subject_s);
  
  return ret;

} // regex_match_capture


/*
  Replace(Pattern,Replacement,String,Result)
  Replace all occurrences of Pattern with Replacement in String

  This is adapted from 
  https://stackoverflow.com/questions/73800119/replace-all-matches-in-pcre2-substitute-in-c

 */
int regex_replace() {

  TERM pattern_p = picat_get_call_arg(1,4);  
  TERM replacement_p = picat_get_call_arg(2,4);
  TERM subject_p = picat_get_call_arg(3,4);
  TERM result_p = picat_get_call_arg(4,4);

 
  char* pattern_s = picat_string_to_cstring(pattern_p);
  char* replacement_s = picat_string_to_cstring(replacement_p);
  char* subject_s = picat_string_to_cstring(subject_p);
  
  int output_size_int = strlen(subject_s);
 
  size_t subject_length = strlen((char *)subject_s);
  size_t replacement_length = strlen((char *)replacement_s);
 
  int errornumber;
  PCRE2_SIZE erroroffset;
  long signed int compile_options = PCRE2_ZERO_TERMINATED;
  pcre2_code *re = pcre2_compile(pattern_s, compile_options, 0, &errornumber, &erroroffset, NULL);
  if (re == NULL) {
    PCRE2_UCHAR buffer[256];
    pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
    printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);

    free(pattern_s);
    free(replacement_s);  
    free(subject_s);

    return PICAT_FALSE;
  }

  // The initial output size might not be enough so we
  // might have to increase the output buffer.
  int output_mult = 1; // times the original output_size
  while (1) {
    int output_size_to_use = output_size_int*output_mult;
    // PCRE2_UCHAR output[output_size_to_use];
    PCRE2_UCHAR *output;
    output = malloc(output_size_to_use * sizeof(PCRE2_UCHAR));
    strcpy(output,"");

    PCRE2_SIZE outlen = output_size_to_use; // sizeof(output) / sizeof(PCRE2_UCHAR);
    // Note: pcre2_substitute adjusts the outlen.
    int rc = pcre2_substitute(re, subject_s, subject_length, 0,
                              PCRE2_SUBSTITUTE_OVERFLOW_LENGTH | PCRE2_SUBSTITUTE_GLOBAL,
                              NULL, NULL,
                              replacement_s, replacement_length, output, &outlen);
    
    if (rc == PCRE2_ERROR_NOMEMORY) {
      // Increase the size of the output buffer and check again.
      output_mult *= 2;
      free(output);
      // printf("output_mult: %d new size: %d\n",output_mult, output_size_to_use);
      
    } else if(rc < 0) {
      // Some error
      PCRE2_UCHAR error_buffer[256];
      pcre2_get_error_message(rc, error_buffer, sizeof(error_buffer));
      printf("PCRE2 substitute error (rc:%d): %s\n", (int)rc, error_buffer);
      
      free(pattern_s);
      free(replacement_s);  
      free(subject_s);
      free(output);
      
      return PICAT_FALSE;
      
    } else {

      picat_unify(result_p,cstring_to_picat((char *)output, strlen(output)));  
      
      free(pattern_s);
      free(replacement_s);  
      free(subject_s);
      free(output);
      
      return PICAT_TRUE;
    }

  }

  return PICAT_FALSE;
  
} // regex_replace


int regex_replace_first() {

  TERM pattern_p = picat_get_call_arg(1,4);  
  TERM replacement_p = picat_get_call_arg(2,4);
  TERM subject_p = picat_get_call_arg(3,4);
  TERM result_p = picat_get_call_arg(4,4);

 
  char* pattern_s = picat_string_to_cstring(pattern_p);
  char* replacement_s = picat_string_to_cstring(replacement_p);
  char* subject_s = picat_string_to_cstring(subject_p);
  
  int output_size_int = strlen(subject_s);
 
  size_t subject_length = strlen((char *)subject_s);
  size_t replacement_length = strlen((char *)replacement_s);
 
  int errornumber;
  PCRE2_SIZE erroroffset;
  long signed int compile_options = PCRE2_ZERO_TERMINATED;
  pcre2_code *re = pcre2_compile(pattern_s, compile_options, 0, &errornumber, &erroroffset, NULL);
  if (re == NULL) {
    PCRE2_UCHAR buffer[256];
    pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
    printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);

    free(pattern_s);
    free(replacement_s);  
    free(subject_s);

    return PICAT_FALSE;
  }

  // The initial output size might not be enough so we
  // might have to increase the output buffer.
  int output_mult = 1; // times the original output_size
  while (1) {
    int output_size_to_use = output_size_int*output_mult;
    // PCRE2_UCHAR output[output_size_to_use];
    PCRE2_UCHAR *output;
    output = malloc(output_size_to_use * sizeof(PCRE2_UCHAR));
    strcpy(output,"");

    PCRE2_SIZE outlen = output_size_to_use; // sizeof(output) / sizeof(PCRE2_UCHAR);
    // Note: pcre2_substitute adjusts the outlen.
    int rc = pcre2_substitute(re, subject_s, subject_length, 0,
                              PCRE2_SUBSTITUTE_OVERFLOW_LENGTH,
                              NULL, NULL,
                              replacement_s, replacement_length, output, &outlen);
    
    if (rc == PCRE2_ERROR_NOMEMORY) {
      // Increase the size of the output buffer and check again.
      output_mult *= 2;
      free(output);
      // printf("output_mult: %d new size: %d\n",output_mult, output_size_to_use);
      
    } else if(rc < 0) {
      // Some error
      PCRE2_UCHAR error_buffer[256];
      pcre2_get_error_message(rc, error_buffer, sizeof(error_buffer));
      printf("PCRE2 substitute error (rc:%d): %s\n", (int)rc, error_buffer);
      
      free(pattern_s);
      free(replacement_s);  
      free(subject_s);
      free(output);
      
      return PICAT_FALSE;
      
    } else {

      picat_unify(result_p,cstring_to_picat((char *)output, strlen(output)));  
      
      free(pattern_s);
      free(replacement_s);  
      free(subject_s);
      free(output);
      
      return PICAT_TRUE;
    }

  }

  return PICAT_FALSE;
  
} // regex_replace_first


/* 
   This is borrowed from PCRE2 distribution's src/pcre2demo.c
   Most is verbatim from the program (including comments), 
   with some small changes.
   
*/
int regex_find_matches() {
  
  pcre2_code *re;
  // PCRE2_SPTR pattern;     /* PCRE2_SPTR is a pointer to unsigned code units of */
  char* pattern;     /* PCRE2_SPTR is a pointer to unsigned code units of */  
  // PCRE2_SPTR subject;     /* the appropriate width (in this case, 8 bits). */
  char* subject;     /* the appropriate width (in this case, 8 bits). */  
  PCRE2_SPTR name_table;
  
  int crlf_is_newline;
  int errornumber; 
  // The number of occurrences to find: 0 indicates to find all.
  int num_to_find = 0; 
  int i;
  int rc;
  int utf8;
  
  uint32_t option_bits;
  uint32_t namecount;
  uint32_t name_entry_size;
  uint32_t newline;
  
  PCRE2_SIZE erroroffset;
  PCRE2_SIZE *ovector;
  PCRE2_SIZE subject_length;
  
  pcre2_match_data *match_data;

  TERM pattern_p     = picat_get_call_arg(1,4);   // The pattern
  TERM subject_p     = picat_get_call_arg(2,4);   // Subject string
  TERM num_to_find_p = picat_get_call_arg(3,4); // Number of matches to find
  TERM output_p      = picat_get_call_arg(4,4);    // Output
  
  pattern = picat_string_to_cstring(pattern_p);
  // printf("pattern: %s\n", pattern);
  subject = picat_string_to_cstring(subject_p);
  num_to_find = picat_get_integer(num_to_find_p);
  // printf("num_to_find: %d\n",num_to_find);
  
  // The list to return
  TERM ret_list = (TERM)NULL;
  TERM ret_list_tail;
  TERM cons;
  TERM picat_match;
  TERM picat_match_tail;
  
  // Number of found matches.
  int num_matches = 0;
  
  /* Pattern and subject are char arguments, so they can be straightforwardly
     cast to PCRE2_SPTR because we are working in 8-bit code units. The subject
     length is cast to PCRE2_SIZE for completeness, though PCRE2_SIZE is in fact
     defined to be size_t. */
  subject_length = (PCRE2_SIZE)strlen((char *)subject);
  
  
  /*************************************************************************
   * Now we are going to compile the regular expression pattern, and handle *
   * any errors that are detected.                                          *
   *************************************************************************/  
  re = pcre2_compile(
                     pattern,               /* the pattern */
                     PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
                     0,                     /* default options */
                     &errornumber,          /* for error number */
                     &erroroffset,          /* for error offset */
                     NULL);                 /* use default compile context */
  
  /* Compilation failed: print the error message and exit. */
  if (re == NULL) {
    PCRE2_UCHAR buffer[256];
    pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
    printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset,
           buffer);
    return PICAT_FALSE;
  }
    
  /*************************************************************************
   * If the compilation succeeded, we call PCRE2 again, in order to do a    *
   * pattern match against the subject string. This does just ONE match. If *
   * further matching is needed, it will be done below. Before running the  *
   * match we must set up a match_data block for holding the result. Using  *
   * pcre2_match_data_create_from_pattern() ensures that the block is       *
   * exactly the right size for the number of capturing parentheses in the  *
   * pattern. If you need to know the actual size of a match_data block as  *
   * a number of bytes, you can find it like this:                          *
   *                                                                        *
   * PCRE2_SIZE match_data_size = pcre2_get_match_data_size(match_data);    *
   *************************************************************************/
  match_data = pcre2_match_data_create_from_pattern(re, NULL);
  
  /* Now run the match. */
  rc = pcre2_match(
                   re,                   /* the compiled pattern */
                   subject,              /* the subject string */
                   subject_length,       /* the length of the subject */
                   0,                    /* start at offset 0 in the subject */
                   0,                    /* default options */
                   match_data,           /* block for storing the result */
                   NULL);                /* use default match context */
  
  /* Matching failed: handle error cases */
  // printf("RC: %d\n",rc);
  if (rc < 0) { 
    switch(rc) {
      // case PCRE2_ERROR_NOMATCH: printf("No match\n"); break;
      case PCRE2_ERROR_NOMATCH: break;        
        /*
          Handle other special cases if you like
        */
      default: printf("Matching error %d\n", rc); break;
    }
    pcre2_match_data_free(match_data);   /* Release memory used for the match */
    pcre2_code_free(re);                 /*   data and the compiled pattern. */
    free(subject);
    return PICAT_FALSE;
  }
  
  /* Match succeeded. Get a pointer to the output vector, where string offsets
     are stored. */
  ovector = pcre2_get_ovector_pointer(match_data);
  // printf("Match succeeded at offset %d\n", (int)ovector[0]);
  
  
  /*************************************************************************
   * We have found the first match within the subject string. If the output *
   * vector wasn't big enough, say so. Then output any substrings that were *
   * captured.                                                              *
   *************************************************************************/
  
  /* The output vector wasn't big enough. This should not happen, because we used
     pcre2_match_data_create_from_pattern() above. */
  if (rc == 0) {
    printf("ovector was not big enough for all the captured substrings\n");
  }
  
  /* Since release 10.38 PCRE2 has locked out the use of \K in lookaround
     assertions. However, there is an option to re-enable the old behaviour. If that
     is set, it is possible to run patterns such as /(?=.\K)/ that use \K in an
     assertion to set the start of a match later than its end. In this demonstration
     program, we show how to detect this case, but it shouldn't arise because the
     option is never set. */
  if (ovector[0] > ovector[1]) {
    printf("\\K was used in an assertion to set the match start after its end.\n"
             "From end to start the match was: %.*s\n", (int)(ovector[0] - ovector[1]),
           (char *)(subject + ovector[1]));
    printf("Run abandoned\n");
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
    free(subject);
    return PICAT_FALSE;
  }

  // printf("First loop RC: %d\n",rc);

  // This is the first match
  picat_match = (TERM)NULL;
  
  /* Show substrings stored in the output vector by number. Obviously, in a real
     application you might want to do things other than print them. */
  // Note: i = 0 is the complete string from first match postition to the last
  // but this is not needed here if there are capture group(s), and we start at i = 1.
  // In the case of no capture group (rc==1) then we get that complete string (i = 0)
  int ix_from = 1;
  int ix_to = rc-1;
  if (rc == 1) {
    // No capture group.
    ix_from = 0;
    ix_to = 0;
  }
  // printf("rc: %d ix_from: %d ix_to: %d\n", rc, ix_from,ix_to);
  for (i = ix_from; i <= ix_to; i++) {
    PCRE2_SPTR substring_start = subject + ovector[2*i];
    PCRE2_SIZE substring_length = ovector[2*i+1] - ovector[2*i];
    // printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);

    char matched[substring_length];
    strncpy(matched,substring_start,substring_length);
    matched[substring_length] = '\0';
    // printf("adding (first loop): %s len: %ld\n",matched, substring_length);    

    // We only increment car/cdr after the first match.
    cons = picat_build_list();
    if (rc > 2) {
      // More than one capture group
      picat_unify(picat_get_car(cons), cstring_to_picat((char*) matched, (int)substring_length));
    } else {
      // No or one capture group
      picat_unify(cons, cstring_to_picat((char*) matched, (int)substring_length));      
    }
    if (picat_match == (TERM)NULL) {
      picat_match = cons;
    } else {
      picat_unify(picat_match_tail, cons);
    }
    picat_match_tail = picat_get_cdr(cons);

  }

  picat_unify(picat_match_tail, picat_build_nil()); // close first match

  
  cons = picat_build_list();
  picat_unify(picat_get_car(cons), picat_match); // place firs match in ret_list
  if (ret_list == (TERM)NULL){
    ret_list = cons;
  } else{
    picat_unify(ret_list_tail, cons);
  }
  ret_list_tail = picat_get_cdr(cons);

  num_matches++;
  
  /**************************************************************************
   * That concludes the basic part of this demonstration program. We have    *
   * compiled a pattern, and performed a single match. The code that follows *
   * shows first how to access named substrings, and then how to code for    *
   * repeated matches on the same subject.                                   *
   **************************************************************************/
  
  /* See if there are any named substrings, and if so, show them by name. First
     we have to extract the count of named parentheses from the pattern. */
  (void)pcre2_pattern_info(
                           re,                   /* the compiled pattern */
                           PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
                           &namecount);          /* where to put the answer */

  // hakank: We don't care about named substrings
  if (0) {
    if (namecount == 0) {
      // printf("No named substrings\n");
    
    } else {
      PCRE2_SPTR tabptr;
      // printf("Named substrings\n");
    
      /* Before we can access the substrings, we must extract the table for
         translating names to numbers, and the size of each entry in the table. */
    
      (void)pcre2_pattern_info(
                               re,                       /* the compiled pattern */
                               PCRE2_INFO_NAMETABLE,     /* address of the table */
                               &name_table);             /* where to put the answer */
    
      (void)pcre2_pattern_info(
                               re,                       /* the compiled pattern */
                               PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
                               &name_entry_size);        /* where to put the answer */
    
      /* Now we can scan the table and, for each entry, print the number, the name,
         and the substring itself. In the 8-bit library the number is held in two
         bytes, most significant first. */    
      tabptr = name_table;
      for (i = 0; i < namecount; i++) {
        int n = (tabptr[0] << 8) | tabptr[1];
        /* printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2, */
        /*        (int)(ovector[2*n+1] - ovector[2*n]), subject + ovector[2*n]); */
      
        tabptr += name_entry_size;
      }
    }
  }
  
  /*************************************************************************
   * If the "-g" option was given on the command line, we want to continue  *
   * to search for additional matches in the subject string, in a similar   *
   * way to the /g option in Perl. This turns out to be trickier than you   *
   * might think because of the possibility of matching an empty string.    *
   * What happens is as follows:                                            *
   *                                                                        *
   * If the previous match was NOT for an empty string, we can just start   *
   * the next match at the end of the previous one.                         *
   *                                                                        *
   * If the previous match WAS for an empty string, we can't do that, as it *
   * would lead to an infinite loop. Instead, a call of pcre2_match() is    *
   * made with the PCRE2_NOTEMPTY_ATSTART and PCRE2_ANCHORED flags set. The *
   * first of these tells PCRE2 that an empty string at the start of the    *
   * subject is not a valid match; other possibilities must be tried. The   *
   * second flag restricts PCRE2 to one match attempt at the initial string *
   * position. If this match succeeds, an alternative to the empty string   *
   * match has been found, and we can print it and proceed round the loop,  *
   * advancing by the length of whatever was found. If this match does not  *
   * succeed, we still stay in the loop, advancing by just one character.   *
   * In UTF-8 mode, which can be set by (*UTF) in the pattern, this may be  *
   * more than one byte.                                                    *
   *                                                                        *
   * However, there is a complication concerned with newlines. When the     *
   * newline convention is such that CRLF is a valid newline, we must       *
   * advance by two characters rather than one. The newline convention can  *
   * be set in the regex by (*CR), etc.; if not, we must find the default.  *
   *************************************************************************/
  
  int find_more = 1; 
  if (num_to_find == 0 || num_to_find > 1) {

   /* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
      sequence. First, find the options with which the regex was compiled and extract
      the UTF state. */
   (void)pcre2_pattern_info(re, PCRE2_INFO_ALLOPTIONS, &option_bits);
   utf8 = (option_bits & PCRE2_UTF) != 0;
  
   /* Now find the newline convention and see whether CRLF is a valid newline
      sequence. */ 
   (void)pcre2_pattern_info(re, PCRE2_INFO_NEWLINE, &newline);
   crlf_is_newline = newline == PCRE2_NEWLINE_ANY ||
     newline == PCRE2_NEWLINE_CRLF ||
     newline == PCRE2_NEWLINE_ANYCRLF;
  
   /* Loop for second and subsequent matches */
   for (;;) {
     // printf("find_more: %d\n", find_more);
     if (!find_more) {
       break;
     }
     uint32_t options = 0;                   /* Normally no options */
     PCRE2_SIZE start_offset = ovector[1];   /* Start at end of previous match */
    
     /* If the previous match was for an empty string, we are finished if we are
        at the end of the subject. Otherwise, arrange to run another match at the
        same point to see if a non-empty match can be found. */      
     if (ovector[0] == ovector[1]) {
       if (ovector[0] == subject_length) break;
       options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
     } else {

       /* If the previous match was not an empty string, there is one tricky case to
          consider. If a pattern contains \K within a lookbehind assertion at the
          start, the end of the matched string can be at the offset where the match
          started. Without special action, this leads to a loop that keeps on matching
          the same substring. We must detect this case and arrange to move the start on
          by one character. The pcre2_get_startchar() function returns the starting
          offset that was passed to pcre2_match(). */      
       PCRE2_SIZE startchar = pcre2_get_startchar(match_data);
       if (start_offset <= startchar) {
         if (startchar >= subject_length) break;   /* Reached end of subject.   */
         start_offset = startchar + 1;             /* Advance by one character. */
         if (utf8) {                                /* If UTF-8, it may be more  */
           /*   than one code unit.     */
           for (; start_offset < subject_length; start_offset++)
             if ((subject[start_offset] & 0xc0) != 0x80) break;
         }
       }
     }
    
     /* Run the next matching operation */
     rc = pcre2_match(
                      re,                   /* the compiled pattern */
                      subject,              /* the subject string */
                      subject_length,       /* the length of the subject */
                      start_offset,         /* starting offset in the subject */
                      options,              /* options */
                      match_data,           /* block for storing the result */
                      NULL);                /* use default match context */
    
     /* This time, a result of NOMATCH isn't an error. If the value in "options"
        is zero, it just means we have found all possible matches, so the loop ends.
        Otherwise, it means we have failed to find a non-empty-string match at a
        point where there was a previous empty-string match. In this case, we do what
        Perl does: advance the matching position by one character, and continue. We
        do this by setting the "end of previous match" offset, because that is picked
        up at the top of the loop as the point at which to start again.
       
        There are two complications: (a) When CRLF is a valid newline sequence, and
        the current position is just before it, advance by an extra byte. (b)
        Otherwise we must ensure that we skip an entire UTF character if we are in
        UTF mode. */
     if (rc == PCRE2_ERROR_NOMATCH) {
       if (options == 0) break;                    /* All matches found */
       ovector[1] = start_offset + 1;              /* Advance one code unit */
       if (crlf_is_newline &&                      /* If CRLF is a newline & */
           start_offset < subject_length - 1 &&    /* we are at CRLF, */
           subject[start_offset] == '\r' &&
           subject[start_offset + 1] == '\n')
         ovector[1] += 1;                          /* Advance by one more. */
       else if (utf8)                              /* Otherwise, ensure we */
         {                                         /* advance a whole UTF-8 */
           while (ovector[1] < subject_length)       /* character. */
             {
               if ((subject[ovector[1]] & 0xc0) != 0x80) break;
               ovector[1] += 1;
             }
         }
       continue;    /* Go round the loop again */
     }
      
     /* Other matching errors are not recoverable. */      
     if (rc < 0) {
       printf("Matching error %d\n", rc);
       pcre2_match_data_free(match_data);
       pcre2_code_free(re);
       free(subject);
       return PICAT_FALSE;
     }
      
     /* Match succeeded */
     // printf("\nMatch succeeded again at offset %d\n", (int)ovector[0]);
      
     /* The match succeeded, but the output vector wasn't big enough. This
        should not happen. */
     if (rc == 0) {
       printf("ovector was not big enough for all the captured substrings\n");
     }
      
     /* We must guard against patterns such as /(?=.\K)/ that use \K in an
        assertion to set the start of a match later than its end. In this
        demonstration program, we just detect this case and give up. */      
     if (ovector[0] > ovector[1]) {
       printf("\\K was used in an assertion to set the match start after its end.\n"
              "From end to start the match was: %.*s\n", (int)(ovector[0] - ovector[1]),
              (char *)(subject + ovector[1]));
       printf("Run abandoned\n");
       pcre2_match_data_free(match_data);
       pcre2_code_free(re);
       free(subject);
       return PICAT_FALSE;
     }
     
     // printf("Next loop rc2: %d\n", rc);
     /* As before, show substrings stored in the output vector by number, and then
        also any named substrings. */
     picat_match = (TERM)NULL;

     int ix_from = 1;
     int ix_to = rc-1;
     if (rc == 1) {
       ix_from = 0;
       ix_to = 0;
     }
     for (i = ix_from; i <= ix_to; i++) {
       PCRE2_SPTR substring_start = subject + ovector[2*i];
       size_t substring_length = ovector[2*i+1] - ovector[2*i];
       // printf("i:%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);

       char matched[substring_length];
       strncpy(matched,(char*)substring_start,(int)substring_length);
       matched[substring_length] = '\0';
       // printf("adding: %s len: %ld check: %ld (num_matches: %d)\n",matched, substring_length, strlen(matched),num_matches);

       cons = picat_build_list();
       if (rc > 2) {
         // More than one capture groups
         picat_unify(picat_get_car(cons), cstring_to_picat((char*) matched, (int)substring_length));
       } else {
         // no or one capture group
         picat_unify(cons, cstring_to_picat((char*) matched, (int)substring_length));         
       }
       if (picat_match == (TERM)NULL){
         picat_match = cons;
       } else {
         picat_unify(picat_match_tail, cons);
       }
       picat_match_tail = picat_get_cdr(cons);
       
     }
     picat_unify(picat_match_tail, picat_build_nil());


     num_matches++;
     
     // increment the main ret_list
     cons = picat_build_list();
     picat_unify(picat_get_car(cons), picat_match); // place first match in ret_list
     if (ret_list == (TERM)NULL) {
       ret_list = ret_list_tail;
     } else {
       picat_unify(ret_list_tail, cons);
     }
     ret_list_tail = picat_get_cdr(cons);
     
     if (num_to_find != 0 && num_matches >= num_to_find) {
       find_more = 0;
     }
     
     /*
       // hakank: We don't care about named substrings
       if (namecount == 0) {
       // printf("No named substrings\n");
       } else {
       PCRE2_SPTR tabptr = name_table;
       // printf("Named substrings\n");
       for (i = 0; i < namecount; i++) {
       int n = (tabptr[0] << 8) | tabptr[1];
       // printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
       //        (int)(ovector[2*n+1] - ovector[2*n]), subject + ovector[2*n]); 
       tabptr += name_entry_size;
       }
       }
     */

   }      /* End of loop to find second and subsequent matches */

 } 
  // printf("\n");
  pcre2_match_data_free(match_data);
  pcre2_code_free(re);
  free(subject);

  // printf("num_matches: %d\n", num_matches);
  picat_unify(ret_list_tail, picat_build_nil()); // add tail []
  picat_unify(ret_list, output_p); // output
  
  return PICAT_TRUE;
  

} // regex_find_all
