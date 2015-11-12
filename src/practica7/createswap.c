main()
{
    int fd;
    char buffer[16*4096];

    fd=creat("swap",0640);
    write(fd,buffer,16*4096);
    close(fd);
    exit(0);
}
