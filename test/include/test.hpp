#pragma once
#include <cstdlib>

extern "C" {
    int printf(const char* fmt, ...);
}


#define BODY($BODY) do { $BODY } while(false)
#define EXPR($EXPR) ($EXPR)

#define ESC($SEQ) "\033[" $SEQ "m"
#define RESET "0;"
#define BOLD "1;"
#define STRIKE "2;"
#define ITALIC "3;"
#define ULN "4;"
#define RED "31;"
#define GREEN "32;"
#define YELLOW "33;"
#define BLUE "34;"
#define PURPLE "35;"
#define CYAN "36;"


#define TEST_MAIN(...) \
    int main(__VA_ARGS__)


#define TEST_QUIT_OK EXPR(0)
#define TEST_QUIT_ERR EXPR(1)
#define TEST_QUIT($STATUS) BODY( \
    printf(($STATUS==0)? \
        (ESC(BOLD ULN GREEN) "\nTEST SUCCEED" ESC(RESET) ": <%s>\n\n" ESC(RESET)) : \
        (ESC(BOLD ULN RED) "\nTEST FAILED" ESC(RESET) ": <%s>\n\n" ESC(RESET)), __FILE__); \
    exit($STATUS); \
)

