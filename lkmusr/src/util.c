#include "util.h"

bool write_bin_file(char *file_name,void *buffer,int size)
{
    FILE* file =fopen(file_name, "wb");
    if( (file) == NULL )
    {

            printf("%s (%s)\n", "fopen fails in write_bin_file",file_name);
            return false;

    }
    if( fwrite(buffer, size, 1, file) != 1 )
    {

            printf("%s (%s)\n", "fwrite fails in write_bin_file",file_name);
            fclose(file);
            return false;

    }

    fclose(file);
    return true;
}

bool write_txt_file(char *file_name,void *buffer,int size)
{
    FILE* file =fopen(file_name, "wt");
    if( (file) == NULL )
    {

            printf("%s (%s)\n", "fopen fails in write_txt_file",file_name);
            return false;

    }
    if( fwrite(buffer, size, 1, file) != 1 )
    {

            printf("%s (%s)\n", "fwrite fails in write_txt_file",file_name);
            fclose(file);
            return false;

    }

    fclose(file);
    return true;
}

bool read_bin_file(char *file_name,void *buffer,int size)
{
    FILE* file =fopen(file_name, "rb");
    if( (file) == NULL )
    {

            printf("%s (%s)\n", "fopen fails in read_bin_file",file_name);
            return false;

    }
    if( fread(buffer, size, 1, file) != 1 )
    {

            printf("%s (%s)\n", "fread fails in read_bin_file",file_name);
            fclose(file);
            return false;

    }

    fclose(file);
    return true;
}

bool read_txt_file(char *file_name,char *buffer,int size)
{
    FILE* file =fopen(file_name, "rt");
    if( (file) == NULL )
    {

            printf("%s (%s)\n", "fopen fails in read_txt_file",file_name);
            return false;

    }
    if( fread(buffer, size, 1, file) != 1 )
    {

            printf("%s (%s)\n", "fread fails in read_txt_file",file_name);
            fclose(file);
            return false;

    }

    fclose(file);
    return true;
}





