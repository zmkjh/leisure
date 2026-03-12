// 2026/3/12
#include <string.h>
#include <stdio.h>

#include "resume.h"

void a(const char* ques, const char* tans) {
    set_resume_point();
    char ans[80];
    printf("%s\n", ques);
    scanf("%s", ans);
    int res = strcmp(ans, tans);
    for (int c; c = getchar(), c != '\n' && c != EOF;);
    if (res) jmp_resume_point();
}

int main() {
    const char* q1 = "who are u";
    const char* a1 = "penguin";
    a(q1, a1);

    const char* q2 = "are u a pig";
    const char* a2 = "no";
    a(q2, a2);

    const char* q3 = "are u a penguin";
    const char* a3 = "yes";
    a(q3, a3);

    const char* q4 = "do u like bananas";
    const char* a4 = "no";
    a(q4, a4);

    return 0;
}
