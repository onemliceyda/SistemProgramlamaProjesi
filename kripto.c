#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"

#define MAKSIMUM_KELIME 500
#define KELIME_MAKS_KARAKTER 100
#define boslukKontrol(x) x == '\r' || x == '\n' || x == '\t' || x == ' '
#define boslukSil(x) while(boslukKontrol(*x)) x++
#define boslukSilHesap(x, y) while(boslukKontrol(*x)) { x++; y++; }

typedef enum {
    jSON_STRINGI = 0,
    JSON_OBJESI
} JSONVeriTipi;

typedef struct sJsonObjesi {
    struct sJsonCifti *cift;
    int sayac;
} JSONObjesi;
    
typedef struct sJsonCifti {
    char* anahtar;
    union uJsonDegeri *deger;
    JSONVeriTipi tip;
} JSONCifti;

typedef union uJsonDegeri {
    char* stringDeger;
    struct sJsonObjesi *jsonObjesi;
} JSONDeger;

static JSONObjesi * jsonAyir(char*, int *);
JSONObjesi *jsonParcala(char* jsonString) {
    int denge = 0;
    JSONObjesi *geciciObje = jsonAyir(jsonString, &denge);
    return geciciObje;
}

void jsonBellekBosalt(JSONObjesi *obj) {
    int i;
    
    if(obj == NULL)
        return;
    
    if(obj->cift == NULL) {
        free(obj);
        return;
    }
    
    for(i = 0; i < obj->sayac; i++) {
        if(obj->cift[i].anahtar != NULL)
            free(obj->cift[i].anahtar);
        if(obj->cift[i].deger != NULL) {
            switch(obj->cift[i].tip) {
                case jSON_STRINGI:
                    free(obj->cift[i].deger->stringDeger);
                    break;
                case JSON_OBJESI:
                    jsonBellekBosalt(obj->cift[i].deger->jsonObjesi);
            }
            free(obj->cift[i].deger);
        }
    }
    
}

static int sonrakiString(char* str, char ch) {
    int pos = 0;
    
    if(str == NULL)
        return -1;
    
    while(*str != ch && *str != '\0') {
        str++;
        pos++;
    }
    return (*str == '\0') ? -1 : pos;
}

static JSONObjesi * jsonAyir(char* str, int * denge) {
    
    int _denge = 0;
    JSONObjesi *obj = (JSONObjesi *) malloc(sizeof(JSONObjesi));
    obj->sayac = 1;
    obj->cift = (JSONCifti *) malloc(sizeof(JSONCifti) *1);
    
    while(*str != '\0') {
        boslukSilHesap(str, _denge);
        if(*str == '{') {
            str++;
            _denge++;
        } else if(*str == '"') {
            
            int i = sonrakiString(++str, '"');
            if(i <= 0) {
                jsonBellekBosalt(obj);
                return NULL;
            }
            
            JSONCifti geciciPtr = obj->cift[obj->sayac - 1];
            
            geciciPtr.anahtar = (char *) malloc(sizeof(char) * i+1);
            memcpy(geciciPtr.anahtar, str, i * sizeof(char));
            geciciPtr.anahtar[i] = '\0';
            
            str += i + 1;
            _denge += i + 2;
            
            i = sonrakiString(str, ':');
            if(i == -1)
                return NULL;
            str += i + 1;
            _denge += i + 1;
            
            boslukSilHesap(str, _denge);
            
            if(*str == '{') {
                int _altNesneyiAyirmadanDenge = _denge;
                int cocukNesneBoyutu;
                geciciPtr.deger = (JSONDeger *) malloc(sizeof(JSONDeger));
                geciciPtr.tip = JSON_OBJESI;
                geciciPtr.deger->jsonObjesi = jsonAyir(str, &_denge);
                if(geciciPtr.deger->jsonObjesi == NULL) {
                    jsonBellekBosalt(obj);
                    return NULL;
                }
                cocukNesneBoyutu = _denge - _altNesneyiAyirmadanDenge;
                str += cocukNesneBoyutu;
            } else if(*str == '"') {
                i = sonrakiString(++str, '"');
                if(i == -1) {
                    jsonBellekBosalt(obj);
                    return NULL;
                }
                geciciPtr.deger = (JSONDeger *) malloc(sizeof(JSONDeger));
                geciciPtr.tip = jSON_STRINGI;
                geciciPtr.deger->stringDeger = (char *) malloc(sizeof(char *) * i+1);
                memcpy(geciciPtr.deger->stringDeger, str, i * sizeof(char));
                geciciPtr.deger->stringDeger[i] = '\0';
                str += i + 1;
                _denge += i + 2;
            }
            obj->cift[obj->sayac - 1] = geciciPtr;
            
        } else if (*str == ',') {
            obj->sayac++;
            obj->cift = (JSONCifti *) realloc(obj->cift,obj->sayac * sizeof(JSONCifti));
            str++;
            _denge++;
        } else if (*str == '}') {
            (*denge) += _denge + 1;
            return obj;
        }
    }
    return obj;
}

int main(int argc, char *argv[])
{ 
    //daha sonra ağaçlardaki veriler ile karşılaştıracağımız kelimeler
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

        FILE * fp;
        char * satir = NULL;
        size_t uzunluk = 0;
        ssize_t read;
        char** diziAnahtar = (char ** ) malloc(sizeof(char *) * MAKSIMUM_KELIME * KELIME_MAKS_KARAKTER);
        char** diziDeger = (char ** ) malloc(sizeof(char *) * MAKSIMUM_KELIME * KELIME_MAKS_KARAKTER);


        fp = fopen(".kilit", "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);


        int indis = 0;
        while ((read = getline(&satir, &uzunluk, fp)) != -1) {
                if ((satir[0] != '{' && satir[1] == ' ') ||(satir[0] != '}' && satir[1] == ' '))
                {
                
                    JSONObjesi *json = jsonParcala(satir);
                    diziAnahtar[indis] = json->cift[0].anahtar;
                    diziDeger[indis] = json->cift[0].deger->stringDeger;
                    indis++;
                }
        }
        fclose(fp);
        if (satir)
            free(satir);
        exit(EXIT_SUCCESS);
        
    } else {
        printf("Lutfen bir 'giris_metin' degeri veriniz!\n");
    }
    
    return 0;
}