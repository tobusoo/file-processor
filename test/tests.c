#include <ctest.h>
#include <glib.h>
#include <stdio.h>

#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>

CTEST(rename, get_suffix)
{
    char* string = "image.jpg";
    char* result;
    result = get_suffix(string);
    char* expect = ".jpg";
    ASSERT_STR(expect, result);

    string = "notsuffix";
    result = get_suffix(string);
    expect = NULL;
    ASSERT_STR(expect, result);
}

CTEST(rename, get_new_name)
{
    char* name = "one_piece.txt";
    char* pattern = "file-{*}.*.old";
    char dest[MAX_LEN] = {0};
    char* expect = "file-{one_piece}.txt.old";
    get_new_name(name, pattern, dest);
    ASSERT_STR(expect, dest);

    name = "image.jpg";
    pattern = "*.jpeg";
    char dest1[MAX_LEN] = {0};
    expect = "image.jpeg";
    get_new_name(name, pattern, dest1);
    ASSERT_STR(expect, dest1);

    name = "hellabc.txt";
    pattern = "hello*.txt";
    char dest2[MAX_LEN] = {0};
    expect = "helloabc.txt";
    get_new_name(name, pattern, dest2);
    ASSERT_STR(expect, dest2);

    name = "hellabc.txt.txt";
    pattern = "hell*o.txt";
    char dest3[MAX_LEN] = {0};
    expect = "hellabco.txt";
    get_new_name(name, pattern, dest3);
    ASSERT_STR(expect, dest3);

    name = "cool.txt";
    pattern = "coo?o.txt";
    char dest4[MAX_LEN] = {0};
    expect = "coolo.txt";
    get_new_name(name, pattern, dest4);
    ASSERT_STR(expect, dest4);

    name = "geometry.txt";
    pattern = "????.txt";
    char dest5[MAX_LEN] = {0};
    expect = "geom.txt";
    get_new_name(name, pattern, dest5);
    ASSERT_STR(expect, dest5);

    name = "hellooooo.txt";
    pattern = "hell?.txt";
    char dest6[MAX_LEN] = {0};
    expect = "hello.txt";
    get_new_name(name, pattern, dest6);
    ASSERT_STR(expect, dest6);

    name = "text_file.jpg";
    pattern = "????.*";
    char dest7[MAX_LEN] = {0};
    expect = "text.jpg";
    get_new_name(name, pattern, dest7);
    ASSERT_STR(expect, dest7);
}

CTEST(rename, get_correct_name_default)
{
    FILE* f = fopen("rename_testing_file.txt", "w");
    fclose(f);
    f = fopen("rename_new_name.txt", "w");
    fclose(f);

    Option opt = {.name_register = R_DEFAULT};

    char* old_name = "rename_testing_file.txt";
    char* new_name = "rename_new_name.txt";
    char dest[MAX_LEN];
    char* expect = "rename_new_name (1).txt";
    get_correct_name(old_name, new_name, dest, &opt);
    remove("rename_testing_file.txt");
    remove("rename_new_name.txt");
    ASSERT_STR(expect, dest);
}

CTEST(rename, get_correct_name_upper)
{
    FILE* f = fopen("rename_testing_file.txt", "w");
    fclose(f);
    f = fopen("rename_new_name.txt", "w");
    fclose(f);

    Option opt = {.name_register = R_HIGH};

    char* old_name = "rename_testing_file.txt";
    char* new_name = "rename_new_name.txt";
    char dest[MAX_LEN];
    char* expect = "RENAME_NEW_NAME.txt";
    get_correct_name(old_name, new_name, dest, &opt);
    remove("rename_testing_file.txt");
    remove("rename_new_name.txt");
    ASSERT_STR(expect, dest);
}

CTEST(rename, get_correct_name_lower)
{
    FILE* f = fopen("rename_testing_file.txt", "w");
    fclose(f);
    f = fopen("Rename_New_Name.txt", "w");
    fclose(f);

    Option opt = {.name_register = R_LOW};

    char* old_name = "rename_testing_file.txt";
    char* new_name = "rename_new_name.txt";
    char dest[MAX_LEN];
    char* expect = "rename_new_name.txt";
    get_correct_name(old_name, new_name, dest, &opt);
    remove("rename_testing_file.txt");
    remove("Rename_New_Name.txt");
    ASSERT_STR(expect, dest);
}

CTEST(lexer, check_sample_string)
{
    int result_0 = check_sample_string("*.txt:  *");
    int result_1 = check_sample_string("**.txt:*");
    int result_2 = check_sample_string("?*test.txt:*");
    int result_3 = check_sample_string(
            "12345678901234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789012345678"
            "90123456789012345678901234567890123456789012345678901234567"
            "89012345678901234567890123456789012345678901234567890123456"
            "789012345678901234567890:*");
    int result_4 = check_sample_string("       :*.txt");
    int result_5 = check_sample_string("test.txt:    ");
    int result_6 = check_sample_string("test.txt   *.txt");
    int result_7 = check_sample_string("test.txt : *.txt : *");
    int result_8 = check_sample_string("test.txt 1.bin : one_piece.*");
    int result_9 = check_sample_string("test.txt : *.txt  0");
    int result_10 = check_sample_string("*/.txt:  *");

    int expected_0 = 0;
    int expected_1 = 1;
    int expected_2 = 2;
    int expected_3 = 3;
    int expected_4 = 4;
    int expected_5 = 5;
    int expected_6 = 6;
    int expected_7 = 7;
    int expected_8 = 8;
    int expected_9 = 9;
    int expected_10 = 10;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
    ASSERT_EQUAL(expected_2, result_2);
    ASSERT_EQUAL(expected_3, result_3);
    ASSERT_EQUAL(expected_4, result_4);
    ASSERT_EQUAL(expected_5, result_5);
    ASSERT_EQUAL(expected_6, result_6);
    ASSERT_EQUAL(expected_7, result_7);
    ASSERT_EQUAL(expected_8, result_8);
    ASSERT_EQUAL(expected_9, result_9);
    ASSERT_EQUAL(expected_10, result_10);
}

CTEST(lexer, get_sample)
{
    char sample[] = "*.txt:  *";
    char search_pattern[] = "*.txt";
    char rename_pattern[] = "*";
    sample_parts patterns[10];
    int returned = get_sample(sample, &patterns[3]);
    int search_pattern_result
            = strcmp(search_pattern, patterns[3].search_pattern);
    int rename_pattern_result
            = strcmp(rename_pattern, patterns[3].rename_pattern);
    int expected = 0;
    ASSERT_EQUAL(expected, returned);
    ASSERT_EQUAL(expected, search_pattern_result);
    ASSERT_EQUAL(expected, rename_pattern_result);
}
