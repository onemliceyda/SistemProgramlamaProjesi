/**
 * @file        main.c
 * @description Huffman Kodlama ve Çözümleme
 * @assignment  Proje/Tasarım
 * @author      Muhammed Melih Çelik - G181210023, Ceyda Önemli - G181210107, Hilal Yıldız - B181210015
 **/

//kütüphaneler ve başlık dosyaları
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

//sabit tanımlamalarımız
#define KELIME_SINIR 500
#define KELIME_KARAKTER_SINIR 100
#define KILIT_YOK "Kilit dosyası yok veya bozulmuş!"
#define DOSYA_FORMAT_YANLIS ".kilit dosyası Json dosya formatına uygun değil!"
#define OPSIYON_BOS "Opsiyon değeri boş olmamalı!"
#define HATALI_OPSIYON "Hatalı opsiyon! Lütfen geçerli bir opsiyon belirtiniz -d veya -e"
#define GIRIS_METIN_YOK "Geçerli opsiyonun gerçekleştirilmesi için giriş metni vermeniz gerekli!"
#define CIKIS_METIN_YOK "Geçerli opsiyonun gerçekleşmesi için çıkış metni vermeniz gerekli!"
#define GIRIS_METIN_GECERSIZ "Lütfen geçerli bir giriş metni veriniz!"
#define KRIPTOLANMIS_DOSYA "Kriptolanmış çıkış dosyası oluşturuldu"
#define COZUMLENMIS_DOSYA "Çözümlenmiş çıkış dosyası oluşturuldu"

//okunabilirlik için enum tanımı
typedef enum
{
    jSON_STRINGI = 0,
    JSON_OBJESI
} JSONVeriTipi;

//json çifti ve sayac tutacak yapı
typedef struct sJsonObjesi
{
    struct sJsonCifti *cift;
    int sayac;
} JSONObjesi;

//anahtar ve değeri tutacak yapı
typedef struct sJsonCifti
{
    char *anahtar;
    union uJsonDegeri *deger;
    JSONVeriTipi tip;
} JSONCifti;

//string değer ve obje tutacak birlik
typedef union uJsonDegeri
{
    char *stringDeger;
    struct sJsonObjesi *jsonObjesi;
} JSONDeger;
static JSONObjesi *jsonAyir(char *, int *);

//değer ve anahtar çözümlemek için kullanacağız
JSONObjesi *jsonParcala(char *jsonString)
{
    int denge = 0;
    JSONObjesi *geciciObje = jsonAyir(jsonString, &denge);
    return geciciObje;
}

//belleği etkin kullanabilmek için
void jsonBellekBosalt(JSONObjesi *obj)
{
    //sayaç için
    int i;

    //JsonObjesi null ise
    if (obj == NULL)
        return;

    if (obj->cift == NULL)
    {
        free(obj);
        return;
    }

    //obje sayacı kadar dön
    for (i = 0; i < obj->sayac; i++)
    {
        //anahtar null değil ise
        if (obj->cift[i].anahtar != NULL)
            free(obj->cift[i].anahtar);
        //değer null değil ise
        if (obj->cift[i].deger != NULL)
        {
            //tipi switchle
            switch (obj->cift[i].tip)
            {
                //eğer json stringi ise
            case jSON_STRINGI:
                free(obj->cift[i].deger->stringDeger);
                break;
                //eğer json objesi ise
            case JSON_OBJESI:
                jsonBellekBosalt(obj->cift[i].deger->jsonObjesi);
            }
            //değeri serbest bırak
            free(obj->cift[i].deger);
        }
    }
}

// \0 string sonu ifadesi ile sonraki stringe geçeceğiz
static int sonrakiString(char *str, char ch)
{
    //pozisyonu tutcacak değişken
    int pos = 0;

    //string boş ise
    if (str == NULL)
        return -1;

    //şart sağlanana kadar
    while (*str != ch && *str != '\0')
    {
        str++;
        pos++;
    }
    return (*str == '\0') ? -1 : pos;
    //ternary ile return ettik
}

//karakterlere göre json dosyasında anahtar ve değerleri alacağımız fonksiyon
static JSONObjesi *jsonAyir(char *str, int *denge)
{

    int _denge = 0;
    //Json objesi için yer ayırdık
    JSONObjesi *obj = (JSONObjesi *)malloc(sizeof(JSONObjesi));
    obj->sayac = 1;
    obj->cift = (JSONCifti *)malloc(sizeof(JSONCifti) * 1);

    //string sonuna gelene kadar
    while (*str != '\0')
    {

        while (*str == '\r' || *str == '\n' || *str == '\t' || *str == ' ')
        { //boşluk, alt satır gibi değerleri atlama
            str++;
            _denge++;
        }
        //süslü parantezde ekleme ama değerleri artır
        if (*str == '{')
        {
            str++;
            _denge++;
        }
        //eğer turnak gelirse
        else if (*str == '"')
        {
            //sonrakiString'den dönen değer
            int i = sonrakiString(++str, '"');
            //0'dan küçükse bellek boşalt
            if (i <= 0)
            {
                jsonBellekBosalt(obj);
                return NULL;
            }

            JSONCifti geciciPtr = obj->cift[obj->sayac - 1];

            //anahtar için yer ayırıyoruz
            geciciPtr.anahtar = (char *)malloc(sizeof(char) * i + 1);
            memcpy(geciciPtr.anahtar, str, i * sizeof(char));
            //anahtar sonu için
            geciciPtr.anahtar[i] = '\0';

            str += i + 1;
            _denge += i + 2;

            //anahtar ve değerleri ayırt etmek için : nokta kullandık
            i = sonrakiString(str, ':');
            if (i == -1)
                return NULL;
            str += i + 1;
            _denge += i + 1;

            while (*str == '\r' || *str == '\n' || *str == '\t' || *str == ' ')
            { //boşluk, alt satır gibi değerleri atlama
                str++;
                _denge++;
            }

            //eğer süslü parantez aç karakteri gelirse
            if (*str == '{')
            {
                int _altNesneyiAyirmadanDenge = _denge;
                int cocukNesneBoyutu;
                //değer için yer ayır
                geciciPtr.deger = (JSONDeger *)malloc(sizeof(JSONDeger));
                geciciPtr.tip = JSON_OBJESI;
                //dengeye göre json ayır
                geciciPtr.deger->jsonObjesi = jsonAyir(str, &_denge);
                //eğer obje boş dönerse bellek boşalt
                if (geciciPtr.deger->jsonObjesi == NULL)
                {
                    jsonBellekBosalt(obj);
                    return NULL;
                }
                cocukNesneBoyutu = _denge - _altNesneyiAyirmadanDenge;
                str += cocukNesneBoyutu;
            }
            //eğer tırnak gelirse
            else if (*str == '"')
            {
                //sonrakiString'den dönen değer
                i = sonrakiString(++str, '"');
                //eğer -1 olursa bellek boşalt
                if (i == -1)
                {
                    jsonBellekBosalt(obj);
                    return NULL;
                }
                //değer için yer ayır
                geciciPtr.deger = (JSONDeger *)malloc(sizeof(JSONDeger));
                geciciPtr.tip = jSON_STRINGI;
                //strin değer tutacağı char* kadar yer ayırdık
                geciciPtr.deger->stringDeger = (char *)malloc(sizeof(char *) * i + 1);
                memcpy(geciciPtr.deger->stringDeger, str, i * sizeof(char));
                geciciPtr.deger->stringDeger[i] = '\0';
                str += i + 1;
                _denge += i + 2;
            }
            obj->cift[obj->sayac - 1] = geciciPtr;
        }
        //eğer virgül var ise
        else if (*str == ',')
        {
            //farklı bir anahtar değer ikilisi vardır
            obj->sayac++;
            obj->cift = (JSONCifti *)realloc(obj->cift, obj->sayac * sizeof(JSONCifti));
            str++;
            _denge++;
        }
        //eğer süslü kapanma sembolu varsa obje döneriz
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
    //parametre boş değil ise
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

        //kelime sonuna kadar
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
    //indis geri dön, daha sonra bu değeri kullanacağız
    return indis;
}

//json dosyası formatını kontrol edecek fonksiyon
void jsonFormatKontrol()
{
    //json dosyasından okunan sembollerin sayısını tutup karşılaştıraracağız, sayı değişkenleri
    int parantezSayisi = 0, tirnakSayisi = 0, ikiNoktaSayisi = 0, satir = 0;
    //okunan string
    char *okunan;
    //inputstruct objesi
    IS jsonDosya;
    //json dosyası için .kilit dosyası kullanıyoruz
    jsonDosya = new_inputstruct(".kilit");
    while (get_line(jsonDosya) >= 0)
    {
        //sayaç
        int i = 0;
        //dosya sonuna gelene kadar
        while (i < jsonDosya->NF)
        {
            okunan = strdup(jsonDosya->fields[i]); //fields'ı al
            //okunan string uzunluğu
            int okunanUzunluk = strlen(okunan);
            for (int j = 0; j < okunanUzunluk; j++)
            {
                //indiste parantez bulursan parantez sayısını artır
                if (okunan[j] == '{' || okunan[j] == '}')
                    parantezSayisi += 1;
                //indiste tırnak bulursan tırnak sayısını artır
                else if (okunan[j] == '"')
                    tirnakSayisi += 1;
                //indiste iki nokta bulursan iki nokta sayısını artır
                else if (okunan[j] == ':')
                    ikiNoktaSayisi += 1;
            }
            i++;
        }
    }
    //satır sayısını alıyoruz
    satir = jsonDosya->line;
    //inputstruct ile işimiz bitti
    jettison_inputstruct(jsonDosya);
    //parantez sayısı 2 değilse , tırnak sayısı satır sayısının 2 eksiğinin 4 katı değil ise ve iki nokta sayısı satır sayısından 2 eksik değil ise hatalıdır
    if (parantezSayisi != 2 && tirnakSayisi != 4 * (satir - 2) && ikiNoktaSayisi != satir - 2)
    {
        //hata varsa program sonlanır
        printf("%s\n", DOSYA_FORMAT_YANLIS);
        exit(EXIT_FAILURE);
    }
    //hata yoksa devam eder
    else
    {
        return;
    }
}

//json dosyasını çözümler diziAnahtar dizisine okunan anahtarları atar indis sayısını geri döner
int jsonDosyasiCozumleAnahtar(char **diziAnahtar)
{
    //indis sayısı tanımı
    int indisJsonAnahtar = 0;
    FILE *fp;
    //satır null başlattık
    char *satir = NULL;
    size_t uzunluk = 0, read;
    //.kilit dosyası açıldı
    fp = fopen(".kilit", "r");

    //satır kalmayınca kadar döngü devam eder
    while ((read = getline(&satir, &uzunluk, fp)) != -1)
    {
        //ilk ve son satırda süslü parantezli satırı parse etmesin diye ek kontrol
        if ((satir[0] != '{' && satir[1] == ' ') || (satir[0] != '}' && satir[1] == ' '))
        {
            //json'ı parçalıyoruz
            JSONObjesi *json = jsonParcala(satir);
            //gelen anahtarı, diziAnahtar dizisinde saklıyoruz
            diziAnahtar[indisJsonAnahtar] = json->cift[0].anahtar;
            //bir değer daha sakladığımız için indis sayısını 1 artırdık
            indisJsonAnahtar++;
        }
    }
    //indis sayısını geri dön
    return indisJsonAnahtar;
}

//json dosyasını çözümler diziDeger dizisine okunan değerleri atar indis sayısını geri döner
int jsonDosyasiCozumleDeger(char **diziDeger)
{
    //indis sayısı tanımı
    int indisJsonDeger = 0;
    FILE *fp;
    //satır null başlattık
    char *satir = NULL;
    size_t uzunluk = 0, read;
    //.kilit dosyası açıldı
    fp = fopen(".kilit", "r");

    //satır kalmayınca kadar döngü devam eder
    while ((read = getline(&satir, &uzunluk, fp)) != -1)
    {
        //ilk ve son satırda süslü parantezli satırı parse etmesin diye ek kontrol
        if ((satir[0] != '{' && satir[1] == ' ') || (satir[0] != '}' && satir[1] == ' '))
        {
            //json'ı parçalıyoruz
            JSONObjesi *json = jsonParcala(satir);
            //gelen değeri, diziDeger dizisinde saklıyoruz
            diziDeger[indisJsonDeger] = json->cift[0].deger->stringDeger;
            //bir değer daha sakladığımız için indis sayısını 1 artırdık
            indisJsonDeger++;
        }
    }
    //indis sayısını geri dön
    return indisJsonDeger;
}

//gelen parametreye göre bir çıkış dosyası oluşturur ve çözümlenmiş veriyi içerir
void cozumlenebilirCikisOlustur(char *argv, JRB agac, JRB geciciAgac, char **kelimeDizisi, char **diziAnahtar, char **diziDeger, int indis, int indisJson)
{
    //dosyaya yazmak için
    FILE *fptr;
    //w parametresi ile açıldı
    fptr = fopen(argv, "w");

    //json dosyasındaki veri kadar
    for (int i = 0; i < indisJson; i++)
    {
        //değer, anahtar olarak ağaca ekleme yapıyoruz
        (void)jrb_insert_str(agac, strdup(diziDeger[i]), new_jval_s(diziAnahtar[i]));
    }
    //okunan metindeki veri kadar
    for (int i = 0; i < indis; i++)
    {
        //agac icerisinde okunan değeri bulma işlemi
        geciciAgac = jrb_find_str(agac, kelimeDizisi[i]);
        //bulunamazsa okunan değer
        if (geciciAgac == NULL)
        {
            //çıkışa verilecek
            fprintf(fptr, "%s ", kelimeDizisi[i]);
        }
        //bulunursa agactaki değer
        else
        {
            //dosyaya yaz
            fprintf(fptr, "%s ", geciciAgac->val.s);
        }
    }
}

//gelen parametreye göre bir çıkış dosyası oluşturur ve kriptolanmış veriyi içerir
void kriptolanmisCikisOlustur(char *argv, JRB agac, JRB geciciAgac, char **kelimeDizisi, char **diziAnahtar, char **diziDeger, int indis, int indisJson)
{
    //dosyaya yazmak için
    FILE *fptr;
    //w parametresi ile açıldı
    fptr = fopen(argv, "w");

    //json dosyasındaki veri kadar
    for (int i = 0; i < indisJson; i++)
    {
        //anahtar, değer olarak ağaca ekleme yapıyoruz
        (void)jrb_insert_str(agac, strdup(diziAnahtar[i]), new_jval_s(diziDeger[i]));
    }
    //okunan metindeki veri kadar
    for (int i = 0; i < indis; i++)
    {
        //agac içerisinden okunan değeri bulma işlemi
        geciciAgac = jrb_find_str(agac, kelimeDizisi[i]);
        //bulunamazsa okunan değer
        if (geciciAgac == NULL)
        {
            //çıkışa verilecek
            fprintf(fptr, "%s ", kelimeDizisi[i]);
        }
        //bulunursa agactaki değer
        else
        {
            //dosyaya yazılacak
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
        //.kilit dosya formatını kontrol eder
        jsonFormatKontrol();

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
        //-d veya -e opsiyonlarından biri seçilmemişse
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
