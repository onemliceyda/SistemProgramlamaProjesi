#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

<<<<<<< HEAD
#define KELIME_SINIR 500
#define KELIME_KARAKTER_SINIR 100
#define KILIT_YOK "Kilit dosyası yok veya bozulmuş"
=======
#define MAKSIMUM_KELIME 500
#define KELIME_MAKS_KARAKTER 100
#define boslukKontrol(x) x == '\r' || x == '\n' || x == '\t' || x == ' '
#define boslukSil(x) while(boslukKontrol(*x)) x++
#define boslukSilHesap(x, y) while(boslukKontrol(*x)) { x++; y++; }
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca

typedef enum
{
    jSON_STRINGI = 0,
    JSON_OBJESI
} JSONVeriTipi;

typedef struct sJsonObjesi
{
    struct sJsonCifti *cift;
    int sayac;
} JSONObjesi;
<<<<<<< Updated upstream
=======
<<<<<<< HEAD

typedef struct sJsonCifti
{
    char *anahtar;
=======
>>>>>>> Stashed changes
    
typedef struct sJsonCifti {
    char* anahtar;
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
    union uJsonDegeri *deger;
    JSONVeriTipi tip;
} JSONCifti;

typedef union uJsonDegeri
{
    char *stringDeger;
    struct sJsonObjesi *jsonObjesi;
} JSONDeger;

static JSONObjesi *jsonAyir(char *, int *);
JSONObjesi *jsonParcala(char *jsonString)
{
    int denge = 0;
    JSONObjesi *geciciObje = jsonAyir(jsonString, &denge);
    return geciciObje;
}

void jsonBellekBosalt(JSONObjesi *obj)
{
    int i;
<<<<<<< Updated upstream
    
    if(obj == NULL)
        return;
=======
<<<<<<< HEAD
    if (obj == NULL)
        return;

    if (obj->cift == NULL)
    {
        free(obj);
        return;
    }

    for (i = 0; i < obj->sayac; i++)
    {
        if (obj->cift[i].anahtar != NULL)
=======
    
    if(obj == NULL)
        return;
>>>>>>> Stashed changes
    
    if(obj->cift == NULL) {
        free(obj);
        return;
    }
    
    for(i = 0; i < obj->sayac; i++) {
        if(obj->cift[i].anahtar != NULL)
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
            free(obj->cift[i].anahtar);
        if (obj->cift[i].deger != NULL)
        {
            switch (obj->cift[i].tip)
            {
            case jSON_STRINGI:
                free(obj->cift[i].deger->stringDeger);
                break;
            case JSON_OBJESI:
                jsonBellekBosalt(obj->cift[i].deger->jsonObjesi);
            }
            free(obj->cift[i].deger);
        }
    }
<<<<<<< Updated upstream
    
=======
<<<<<<< HEAD
=======
    
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
>>>>>>> Stashed changes
}

static int sonrakiString(char *str, char ch)
{
    int pos = 0;
<<<<<<< Updated upstream
    
    if(str == NULL)
        return -1;
=======
<<<<<<< HEAD

    if (str == NULL)
        return -1;

    while (*str != ch && *str != '\0')
    {
=======
    
    if(str == NULL)
        return -1;
>>>>>>> Stashed changes
    
    while(*str != ch && *str != '\0') {
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
        str++;
        pos++;
    }
    return (*str == '\0') ? -1 : pos;
}

<<<<<<< Updated upstream
static JSONObjesi * jsonAyir(char* str, int * denge) {
    
=======
<<<<<<< HEAD
static JSONObjesi *jsonAyir(char *str, int *denge)
{

=======
static JSONObjesi * jsonAyir(char* str, int * denge) {
    
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
>>>>>>> Stashed changes
    int _denge = 0;
    JSONObjesi *obj = (JSONObjesi *)malloc(sizeof(JSONObjesi));
    obj->sayac = 1;
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
    obj->cift = (JSONCifti *)malloc(sizeof(JSONCifti) * 1);

    while (*str != '\0')
    {

        while (*str == '\r' || *str == '\n' || *str == '\t' || *str == ' ')
        {
            str++;
            _denge++;
        }
        if (*str == '{')
        {
            str++;
            _denge++;
        }
        else if (*str == '"')
        {

=======
>>>>>>> Stashed changes
    obj->cift = (JSONCifti *) malloc(sizeof(JSONCifti) *1);
    
    while(*str != '\0') {
        boslukSilHesap(str, _denge);
        if(*str == '{') {
            str++;
            _denge++;
        } else if(*str == '"') {
            
<<<<<<< Updated upstream
=======
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
>>>>>>> Stashed changes
            int i = sonrakiString(++str, '"');
            if (i <= 0)
            {
                jsonBellekBosalt(obj);
                return NULL;
            }
            
            JSONCifti geciciPtr = obj->cift[obj->sayac - 1];
<<<<<<< Updated upstream
=======
<<<<<<< HEAD

            geciciPtr.anahtar = (char *)malloc(sizeof(char) * i + 1);
=======
>>>>>>> Stashed changes
            
            geciciPtr.anahtar = (char *) malloc(sizeof(char) * i+1);
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
            memcpy(geciciPtr.anahtar, str, i * sizeof(char));
            geciciPtr.anahtar[i] = '\0';
            
            str += i + 1;
            _denge += i + 2;
            
            i = sonrakiString(str, ':');
            if (i == -1)
                return NULL;
            str += i + 1;
            _denge += i + 1;
<<<<<<< Updated upstream
=======
<<<<<<< HEAD

            while (*str == '\r' || *str == '\n' || *str == '\t' || *str == ' ')
            {
                str++;
                _denge++;
            }

            if (*str == '{')
            {
=======
>>>>>>> Stashed changes
            
            boslukSilHesap(str, _denge);
            
            if(*str == '{') {
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
                int _altNesneyiAyirmadanDenge = _denge;
                int cocukNesneBoyutu;
                geciciPtr.deger = (JSONDeger *)malloc(sizeof(JSONDeger));
                geciciPtr.tip = JSON_OBJESI;
                geciciPtr.deger->jsonObjesi = jsonAyir(str, &_denge);
                if (geciciPtr.deger->jsonObjesi == NULL)
                {
                    jsonBellekBosalt(obj);
                    return NULL;
                }
                cocukNesneBoyutu = _denge - _altNesneyiAyirmadanDenge;
                str += cocukNesneBoyutu;
            }
            else if (*str == '"')
            {
                i = sonrakiString(++str, '"');
                if (i == -1)
                {
                    jsonBellekBosalt(obj);
                    return NULL;
                }
                geciciPtr.deger = (JSONDeger *)malloc(sizeof(JSONDeger));
                geciciPtr.tip = jSON_STRINGI;
                geciciPtr.deger->stringDeger = (char *)malloc(sizeof(char *) * i + 1);
                memcpy(geciciPtr.deger->stringDeger, str, i * sizeof(char));
                geciciPtr.deger->stringDeger[i] = '\0';
                str += i + 1;
                _denge += i + 2;
            }
            obj->cift[obj->sayac - 1] = geciciPtr;
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
        }
        else if (*str == ',')
        {
=======
>>>>>>> Stashed changes
            
        } else if (*str == ',') {
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
            obj->sayac++;
            obj->cift = (JSONCifti *)realloc(obj->cift, obj->sayac * sizeof(JSONCifti));
            str++;
            _denge++;
        }
        else if (*str == '}')
        {
            (*denge) += _denge + 1;
            return obj;
        }
    }
    return obj;
}

int main(int argc, char *argv[])
{ 

    JRB agac;
    JRB basilacakA;
    agac = make_jrb();

    //daha sonra ağaçlardaki veriler ile karşılaştıracağımız kelimeler
    char kelimeDizisi[KELIME_SINIR][KELIME_KARAKTER_SINIR];
    if (argv[2] != NULL)
    {
        //inputstruct oluşturuldu
        IS is;
        //file olarak açıldı
        is = new_inputstruct(argv[2]);
        int indis = 0;
        while (get_line(is) >= 0)
        {
            for (int i = 0; i < is->NF; i++)
            {
                //okunan kelimeler
<<<<<<< Updated upstream
                strcpy(kelimeDizisi[indis],is->fields[i]);   
=======
<<<<<<< HEAD
                strcpy(kelimeDizisi[indis], is->fields[i]);
=======
                strcpy(kelimeDizisi[indis],is->fields[i]);   
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
>>>>>>> Stashed changes
                indis++;
            }
        }  

        FILE *fp;
        char *satir = NULL;
        size_t uzunluk = 0;
        ssize_t read;
<<<<<<< HEAD
        char **diziAnahtar = (char **)malloc(sizeof(char *) * KELIME_SINIR * KELIME_KARAKTER_SINIR);
        char **diziDeger = (char **)malloc(sizeof(char *) * KELIME_SINIR * KELIME_KARAKTER_SINIR);

        fp = fopen(".kilit", "r");
        if (fp == NULL)
        {
            printf("%s", KILIT_YOK);
=======
        char** diziAnahtar = (char ** ) malloc(sizeof(char *) * MAKSIMUM_KELIME * KELIME_MAKS_KARAKTER);
        char** diziDeger = (char ** ) malloc(sizeof(char *) * MAKSIMUM_KELIME * KELIME_MAKS_KARAKTER);

        fp = fopen(".kilit", "r");
        if (fp == NULL)
<<<<<<< Updated upstream
=======
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
>>>>>>> Stashed changes
            exit(EXIT_FAILURE);
        int indisJson = 0;
        while ((read = getline(&satir, &uzunluk, fp)) != -1)
        {
            if ((satir[0] != '{' && satir[1] == ' ') || (satir[0] != '}' && satir[1] == ' '))
            {
                JSONObjesi *json = jsonParcala(satir);
                diziAnahtar[indisJson] = json->cift[0].anahtar;
                diziDeger[indisJson] = json->cift[0].deger->stringDeger;
                indisJson++;
            }
        }
        //dosyaya yazmak için
        FILE *fptr;
        //w parametresi ile açıldı
        fptr = fopen("cikis_metin", "w");

        //çözümlenebilir dosya oluşturulacak burada
        if (strstr(argv[1], "-d") != NULL)
        {
            //kilit dosyasından okunan {kelime, kod} ikilisi JRB ağaçta -> {val, key} olarak
            for (int i = 0; i < indisJson; i++)
            {
                (void)jrb_insert_str(agac, strdup(diziDeger[i]), new_jval_s(diziAnahtar[i]));
            }
            for (int i = 0; i < indis; i++)
            {
                basilacakA = jrb_find_str(agac, kelimeDizisi[i]);
                if (basilacakA == NULL)
                {
                    //çıkışa verilecek
                    printf("%s ", kelimeDizisi[i]);
                }
                else
                {
                    //dosyaya yaz
                    fprintf(fptr, "%s ", basilacakA->val.s);
                }
            }
        }

<<<<<<< Updated upstream
        //çözümlenebilir dosya oluşturulacak burada
        if (strstr(argv[1],"-d") != NULL)
        {
            //kilit dosyasından okunan {kelime, kod} ikilisi JRB ağaçta -> {val, key} olarak
            for (int i = 0; i < indisJson; i++)
            {
                (void) jrb_insert_str(agac,strdup(diziDeger[i]),new_jval_s(diziAnahtar[i]));
            }
			for (int i = 0; i < indis; i++)
            {
                basilacakA = jrb_find_str(agac,kelimeDizisi[i]);
                if (basilacakA != NULL)
                {
                    printf("%s\n",kelimeDizisi[i]);
                }else{
                    //fprintf(fptr,"%s",basilacakA->val.s);
                    //dosyaya yaz
                    printf("%s \n",basilacakA->val.s);
                }
            }

        }
        //kilitli dosya oluşturulacak burada
        else if (strcmp(argv[1],"-e") == 0)
        {
            //kilit dosyasından okunan {kelime, kod} ikilisi JRB ağaçta -> {key, val} şeklinde konulacaktır
            for (int i = 0; i < indisJson; i++)
            {
                (void) jrb_insert_str(agac,strdup(diziAnahtar[i]),new_jval_s(diziDeger[i])); 
            }
        } else {
            printf("Lütfen geçerli bir opsiyon seçiniz");
        }
=======
<<<<<<< HEAD
        //kilitli dosya oluşturulacak burada
        else if (strcmp(argv[1], "-e") == 0)
        {
            //kilit dosyasından okunan {kelime, kod} ikilisi JRB ağaçta -> {key, val} şeklinde konulacaktır
            for (int i = 0; i < indisJson; i++)
            {
                (void)jrb_insert_str(agac, strdup(diziAnahtar[i]), new_jval_s(diziDeger[i]));
            }
            for (int i = 0; i < indis; i++)
            {
                basilacakA = jrb_find_str(agac, kelimeDizisi[i]);
                if (basilacakA == NULL)
                {
                    printf("%s ", kelimeDizisi[i]);
                }
                else
                {
                    //dosyaya yazılacak
                    //printf("%s \n",basilacakA->val.s);
                    fprintf(fptr, "%s ", basilacakA->val.s);
                }
            }
        }
        else
        {
            printf("Lütfen geçerli bir opsiyon seçiniz");
        }

=======
        //çözümlenebilir dosya oluşturulacak burada
        if (strstr(argv[1],"-d") != NULL)
        {
            //kilit dosyasından okunan {kelime, kod} ikilisi JRB ağaçta -> {val, key} olarak
            for (int i = 0; i < indisJson; i++)
            {
                (void) jrb_insert_str(agac,strdup(diziDeger[i]),new_jval_s(diziAnahtar[i]));
            }
			for (int i = 0; i < indis; i++)
            {
                basilacakA = jrb_find_str(agac,kelimeDizisi[i]);
                if (basilacakA != NULL)
                {
                    printf("%s\n",kelimeDizisi[i]);
                }else{
                    //fprintf(fptr,"%s",basilacakA->val.s);
                    //dosyaya yaz
                    printf("%s \n",basilacakA->val.s);
                }
            }
>>>>>>> Stashed changes

        }
        //kilitli dosya oluşturulacak burada
        else if (strcmp(argv[1],"-e") == 0)
        {
            //kilit dosyasından okunan {kelime, kod} ikilisi JRB ağaçta -> {key, val} şeklinde konulacaktır
            for (int i = 0; i < indisJson; i++)
            {
                (void) jrb_insert_str(agac,strdup(diziAnahtar[i]),new_jval_s(diziDeger[i])); 
            }
        } else {
            printf("Lütfen geçerli bir opsiyon seçiniz");
        }

>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
        fclose(fp);
        if (satir)
            free(satir);
        exit(EXIT_SUCCESS);
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
    }
    else
    {
=======
>>>>>>> Stashed changes
        
    } else {
>>>>>>> 06ac950148a25b120d1b293a38d7f71c054a49ca
        printf("Lutfen bir 'giris_metin' degeri veriniz!\n");
    }
    
    return 0;
}