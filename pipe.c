void readFromPipe(int in, char *fileName){
	FILE *stream;
	int c;
	int file;
	stream = fdopen(in, "r");
	if((file = open(fileName, O_WRONLY & O_CREAT & (S IRUSR | S IWUSR | S IRGRP | S IROTH))) > 0)
	{
		while((c=fgetc(stream)) != EOF)
			write(file, c, 1);
	}
}
