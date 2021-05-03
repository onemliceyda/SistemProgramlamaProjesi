#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"

#define MAKSIMUM_KELIME 500
#define KELIME_MAKS_KARAKTER 100

int main(int argc, char *argv[])
{ 
    //daha sonra ağaçlardaki veriler ile karşılaştıracağımız okunan kelimeler
    char kelimeDizisi[MAKSIMUM_KELIME][KELIME_MAKS_KARAKTER];
    if (argv[2] != NULL)
    {
        int indis = 0;
        //inputstruct oluşturuldu
        IS is;
        //file olarak açıldı
        is = new_inputstruct(argv[2]);
        while (get_line(is) >= 0)
        {
            for (int i = 0; i < is->NF; i++)
            {
                //okunan kelimeler
                strcpy(kelimeDizisi[indis],is->fields[i]);   
                indis++;
            }
        }   
    } else {
        printf("Lutfen bir 'giris_metin' degeri veriniz!\n");
    }
    
    return 0;
}
