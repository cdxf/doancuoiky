#include "syscall.h"

int
main()
{
    SpaceId newProc;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    char prompt[2], ch, buffer[60];
    int temp;

    prompt[0] = '-';
    prompt[1] = '-';

    Write("\nChuong trinh thu nghiem cac system call:\n", 50, output);
    Write("1.CreateFile: system call dung de tao file.\n", 50, output);
    Write("2.Open: system call dung de moi file.\n", 50, output);
    Write("3.Close: system call dung de dong file.\n", 50, output);
    Write("4.Exec system call dung de thuc thi mot chuong trinh.\n", 50, output);
    Write("Hay lua chon system call de thu nghiem <an q de thoat>: ");
    Read(&ch, 1, input);
    /*while( 1 )
    {
	Write(prompt, 2, output);

	i = 0;

	do {

	    Read(&buffer[i], 1, input);

	} while( buffer[i++] != '\n' );

	buffer[--i] = '\0';

	if( i > 0 ) {
		newProc = Exec(buffer);
		Join(newProc);
	}
    }
  int MaxFileLength = 31;
  char buff[MaxFileLength];
	Write("\n**Chuong trinh creatfile **\n",50,ConsoleOutput);
	Write("\nBan hay ten file :",30,ConsoleOutput);
	Read(buff,MaxFileLength,ConsoleInput);
	//CreateFile(buff);
	Exit(0);   */
//	return 0;
}

