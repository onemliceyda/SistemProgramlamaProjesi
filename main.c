#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

#define KELIME_SINIR 500
#define KELIME_KARAKTER_SINIR 100
#define KILIT_YOK "Kilit dosyası yok veya bozulmuş"

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

typedef struct sJsonCifti
{
    char *anahtar;
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
}

static int sonrakiString(char *str, char ch)
{
    int pos = 0;

    if (str == NULL)
        return -1;

    while (*str != ch && *str != '\0')
    {
        str++;
        pos++;
    }
    return (*str == '\0') ? -1 : pos;
}

static JSONObjesi *jsonAyir(char *str, int *denge)
{

    int _denge = 0;
    JSONObjesi *obj = (JSONObjesi *)malloc(sizeof(JSONObjesi));
    obj->sayac = 1;
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

            int i = sonrakiString(++str, '"');
            if (i <= 0)
            {
                jsonBellekBosalt(obj);
                return NULL;
            }

            JSONCifti geciciPtr = obj->cift[obj->sayac - 1];

            geciciPtr.anahtar = (char *)malloc(sizeof(char) * i + 1);
            memcpy(geciciPtr.anahtar, str, i * sizeof(char));
            geciciPtr.anahtar[i] = '\0';

            str += i + 1;
            _denge += i + 2;

            i = sonrakiString(str, ':');
            if (i == -1)
                return NULL;
            str += i + 1;
            _denge += i + 1;

            while (*str == '\r' || *str == '\n' || *str == '\t' || *str == ' ')
            {
                str++;
                _denge++;
            }

            if (*str == '{')
            {
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
        }
        else if (*str == ',')
        {
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
                strcpy(kelimeDizisi[indis], is->fields[i]);
                indis++;
            }
        }

        FILE *fp;
        char *satir = NULL;
        size_t uzunluk = 0;
        ssize_t read;
        char **diziAnahtar = (char **)malloc(sizeof(char *) * KELIME_SINIR * KELIME_KARAKTER_SINIR);
        char **diziDeger = (char **)malloc(sizeof(char *) * KELIME_SINIR * KELIME_KARAKTER_SINIR);

        fp = fopen(".kilit", "r");
        if (fp == NULL)
        {
            printf("%s", KILIT_YOK);
            exit(EXIT_FAILURE);
        }
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

        fclose(fp);
        if (satir)
            free(satir);
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("Lutfen bir 'giris_metin' degeri veriniz!\n");
    }

    return 0;
}