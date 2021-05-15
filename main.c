#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

#define KELIME_SINIR 500
#define KELIME_KARAKTER_SINIR 100
#define KILIT_YOK "Kilit dosyası yok veya bozulmuş!"
#define OPSIYON_BOS "Opsiyon değeri boş olmamalı!"
#define HATALI_OPSIYON "Hatalı opsiyon! Lütfen geçerli bir opsiyon belirtiniz -d veya -e"
#define GIRIS_METIN_YOK "Geçerli opsiyonun gerçekleştirilmesi için giriş metni vermeniz gerekli!"
#define CIKIS_METIN_YOK "Geçerli opsiyonun gerçekleşmesi için çıkış metni vermeniz gerekli!"
#define GIRIS_METIN_GECERSIZ "Lütfen geçerli bir giriş metni veriniz!"
#define KRIPTOLANMIS_DOSYA "Kriptolanmış çıkış dosyası oluşturuldu"
#define COZUMLENMIS_DOSYA "Çözümlenmiş çıkış dosyası oluşturuldu"

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

//fields ile giriş metnini okur kelime dizisine değerler atanır
int girisMetinOku(char **kelimeDizisi, char *argv, int indis, IS is)
{
    if (argv != NULL)
    {
        //file olarak açıldı
        is = new_inputstruct(argv);

        //giriş metin gerçekten var mı kontrol
        if (is == NULL)
        {
            printf("%s\n", GIRIS_METIN_GECERSIZ);
            exit(1);
        }

        while (get_line(is) >= 0)
        {
            for (int i = 0; i < is->NF; i++)
            {
                //okunan kelimeler
                kelimeDizisi[indis] = strdup(is->fields[i]);
                //okununa indis artışı
                indis++;
            }
        }
    }
    //sonlandırmak için
    jettison_inputstruct(is);
    return indis;
}

//json dosyasını çözümler diziAnahtar dizisine okunan anahtarları atar indeks sayısını geri döner
int jsonDosyasiCozumleAnahtar(char **diziAnahtar)
{
    int indisJsonAnahtar = 0;
    FILE *fp;
    char *satir = NULL;
    size_t uzunluk = 0;
    ssize_t read;
    fp = fopen(".kilit", "r");

    while ((read = getline(&satir, &uzunluk, fp)) != -1)
    {
        if ((satir[0] != '{' && satir[1] == ' ') || (satir[0] != '}' && satir[1] == ' '))
        {
            JSONObjesi *json = jsonParcala(satir);
            diziAnahtar[indisJsonAnahtar] = json->cift[0].anahtar;
            indisJsonAnahtar++;
        }
    }
    return indisJsonAnahtar;
}

//json dosyasını çözümler diziDeger dizisine okunan değerleri atar indeks sayısını geri döner
int jsonDosyasiCozumleDeger(char **diziDeger)
{
    int indisJsonDeger = 0;
    FILE *fp;
    char *satir = NULL;
    size_t uzunluk = 0;
    ssize_t read;
    fp = fopen(".kilit", "r");

    while ((read = getline(&satir, &uzunluk, fp)) != -1)
    {
        if ((satir[0] != '{' && satir[1] == ' ') || (satir[0] != '}' && satir[1] == ' '))
        {
            JSONObjesi *json = jsonParcala(satir);
            diziDeger[indisJsonDeger] = json->cift[0].deger->stringDeger;
            indisJsonDeger++;
        }
    }
    return indisJsonDeger;
}

//çözümlenebilir dosya oluşturulacak burada
void cozumlenebilirCikisOlustur(char *argv, JRB agac, JRB geciciAgac, char **kelimeDizisi, char **diziAnahtar, char **diziDeger, int indis, int indisJson)
{
    //dosyaya yazmak için
    FILE *fptr;
    //w parametresi ile açıldı
    fptr = fopen(argv, "w");

    //kilit dosyasından okunan {kelime, kod} ikilisi JRB ağaçta -> {val, key} olarak
    for (int i = 0; i < indisJson; i++)
    {
        (void)jrb_insert_str(agac, strdup(diziDeger[i]), new_jval_s(diziAnahtar[i]));
    }
    for (int i = 0; i < indis; i++)
    {
        geciciAgac = jrb_find_str(agac, kelimeDizisi[i]);
        if (geciciAgac == NULL)
        {
            //çıkışa verilecek
            fprintf(fptr, "%s ", kelimeDizisi[i]);
        }
        else
        {
            //dosyaya yaz
            fprintf(fptr, "%s ", geciciAgac->val.s);
        }
    }
}

void kriptolanmisCikisOlustur(char *argv, JRB agac, JRB geciciAgac, char **kelimeDizisi, char **diziAnahtar, char **diziDeger, int indis, int indisJson)
{
    //dosyaya yazmak için
    FILE *fptr;
    //w parametresi ile açıldı
    fptr = fopen(argv, "w");
    for (int i = 0; i < indisJson; i++)
    {
        (void)jrb_insert_str(agac, strdup(diziAnahtar[i]), new_jval_s(diziDeger[i]));
    }
    for (int i = 0; i < indis; i++)
    {
        geciciAgac = jrb_find_str(agac, kelimeDizisi[i]);
        if (geciciAgac == NULL)
        {
            //çıkışa verilecek
            fprintf(fptr, "%s ", kelimeDizisi[i]);
        }
        else
        {
            //dosyaya yazılacak
            //printf("%s \n",basilacakA->val.s);
            fprintf(fptr, "%s ", geciciAgac->val.s);
        }
    }
}

//bellekte yer tahsis eden tek boyutlu diziler serbest bırakılacak
void tekDiziSerbest(char *dizi)
{
    free(dizi);
}

//bellekte yer tahsis eden cok boyutlu diziler serbest bırakılacak
void cokDiziSerbest(char **dizi)
{
    free(dizi);
}

//inputstruct serbest bırakılacak
static void libfdrSerbest(IS is)
{
    jettison_inputstruct(is);
}

int main(int argc, char *argv[])
{

    //inputstruct oluştur
    IS isGiris, isJson;

    //ön parametre kontrolü
    isJson = new_inputstruct(".kilit");
    if (isJson == NULL) //kilit dosyası yok ise
    {
        printf("%s\n", KILIT_YOK);
        exit(EXIT_FAILURE);
    }
    else if (argv[1] == NULL) //opsiyon boş ise
    {
        printf("%s\n", OPSIYON_BOS);
        exit(EXIT_FAILURE);
    }
    else if (argv[2] == NULL) //giriş metni parametresi boş ise
    {
        printf("%s\n", GIRIS_METIN_YOK);
        exit(EXIT_FAILURE);
    }
    else if (argv[3] == NULL) //çıkış metni parametresi boş ise
    {
        printf("%s\n", CIKIS_METIN_YOK);
        exit(EXIT_FAILURE);
    }
    else
    {

        //giriş metni okunurken indisi tutacağız
        int indis = 0;

        //ağaç örneği
        JRB agac;
        //geçici ağaç
        JRB geciciAgac;
        ///jrb ağaç oluştur
        agac = make_jrb();

        //boyut için
        char *kelimeKarakterSinir = (char *)malloc((sizeof(char) * KELIME_KARAKTER_SINIR));
        //veri tutucu string diziler
        //giriş metninin kelimelerini tutacak dizi
        char **kelimeDizisi = (char **)malloc(sizeof(kelimeKarakterSinir) * KELIME_SINIR);
        //.kilit dosyası anahtarları tutacağımız dizi sonra ağaca ekleyeceğiz
        char **diziAnahtar = (char **)malloc(sizeof(kelimeKarakterSinir) * KELIME_SINIR);
        //.kilit dosyası değerleri tutacağımız dizi sonra ağaca ekleyeceğiz
        char **diziDeger = (char **)malloc(sizeof(kelimeKarakterSinir) * KELIME_SINIR);

        int indisJsonAnahtar = 0;
        //json dosyası okundu anahtarlar diziAnahtar'a atandı indeks geri dönüldü
        indisJsonAnahtar = jsonDosyasiCozumleAnahtar(diziAnahtar);

        int indisJsonDeger = 0;
        //json dosyası okundu değerler diziDeger'e atandı indeks geri dönüldü
        indisJsonDeger = jsonDosyasiCozumleDeger(diziDeger);

        //giriş metni okundu kelime dizisi olarak, kelimeDizisine atandı
        indis = girisMetinOku(kelimeDizisi, argv[2], indis, isGiris);

        //orijinal metindeki kelimeler yerine boşlukla ayrılmış Huffman kodları yazılmış metin tabanlı (ASCII) dosyadır, ağaç oluşturur, {kelime, kod} ikilisi JRB ağaçta -> {key, val} şeklinde eklendi
        if (strcmp(argv[1], "-e") == 0) //gelen parametre -e ise
        {
            //kilit dosyasında olmayan bir kelime karşılaşılırsa kelimenin orijinali çıkış dosyasına yazılıyor
            kriptolanmisCikisOlustur(argv[3], agac, geciciAgac, kelimeDizisi, diziAnahtar, diziDeger, indis, indisJsonDeger);
            printf("%s\n", KRIPTOLANMIS_DOSYA);
            exit(EXIT_SUCCESS);
        }
        //bir kilit kullanılarak çözümlenmiş okunabilir metin (ASCII) dosyadır, ağaç oluşturur, {kelime, kod} ikilisi JRB ağaçta -> {val, key} olarak eklendi
        else if (strcmp(argv[1], "-d") == 0) //gelen parametre -d ise
        {
            //kriptolanmış dosya içinde ağaçta olmayan bir kelimeye rastlanırsa, kriptolanmış kelime çıkışa yazıldı
            cozumlenebilirCikisOlustur(argv[3], agac, geciciAgac, kelimeDizisi, diziAnahtar, diziDeger, indis, indisJsonAnahtar);
            printf("%s\n", COZUMLENMIS_DOSYA);
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("%s\n", HATALI_OPSIYON);
            exit(EXIT_FAILURE);
        }

        //tahsis edilen bellek geri bırakıldı
        tekDiziSerbest(kelimeKarakterSinir);
        cokDiziSerbest(kelimeDizisi);
        cokDiziSerbest(diziAnahtar);
        cokDiziSerbest(diziDeger);
        //jettison fonksiyonlar içerisinde görevi bittikçe kullanıldı
        libfdrSerbest(isJson);
    }
    return 0;
}
