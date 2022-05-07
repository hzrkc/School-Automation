#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>             // Hazar KOÇ 200202007

struct kayit
{
    int ogrNo;
    int dersKodu;
    int puan;
    char ad[20];
    char soyad[20];
};

void indexOlustur(char *dataFilePath);
void SortKayit(int *kayitlar, int *offsets, int kayitSayisi);
struct kayit *KayitOku(FILE *dataFile);
void indexDosyasiniGoster(char *indexFilePath);
void kayiEkle(char *dataFilePath, char *indexFilePath);
void indexDosyasiniSil(char *fileName);
void veriDosyasiniGoster(char *dataFilePath);
int kayitBul(char *dataFilePath, char *indexFilePath);
void kayitSil(char *dataFilePath, char *indexFilePath);
void kayitGuncelle(char *dataFilePath, char *indexFilePath);

int main(void)
{
    char *dataFilePath = "data.bin";
    char *indexFilePath = "index.txt";
    int k = 1;
    int cont = 0;
    
    while (k != 0)
    {
        if(cont > 0){
            printf("Devam etmek icin enter'a basiniz ");
            getchar();
            getchar();
        }
        cont++;
        printf("\nOgrenci kayit programina hos geldiniz.\n\n");
        printf("Lutfen bir secim yapiniz.\n\n");
        printf("Index dosyasi olusturmak icin:\t\t1 giriniz.\n\n");
        printf("Kayit eklemek icin:\t\t\t2 giriniz.\n\n");
        printf("Kayit bulmak icin:\t\t\t3 giriniz.\n\n");
        printf("Kayit silmek icin:\t\t\t4 giriniz.\n\n");
        printf("Kayit guncellemek icin:\t\t\t5 giriniz.\n\n");
        printf("Veri dosyasini goruntulemek icin:\t6 giriniz.\n\n");
        printf("Index dosyasini goruntulemek icin:\t7 giriniz.\n\n");
        printf("Index dosyasini silmek icin:\t\t8 giriniz.\n\n");
        printf("Programi kapatmak icin:\t\t\t0 giriniz.\n\n");
        printf("Giris: ");
        scanf(" %d", &k);
        switch (k)
        {
        case 0:
            printf("Program sonlandi. Iyi gunler");
            break;
        case 1:
            indexOlustur("data.bin");
            break;
        case 2:
            kayiEkle(dataFilePath, indexFilePath);
            break;
        case 3:
            kayitBul(dataFilePath, indexFilePath);
            break;
        case 4:
            kayitSil(dataFilePath, indexFilePath);
            break;
        case 5:
            kayitGuncelle(dataFilePath, indexFilePath);
            break;
        case 6:
            veriDosyasiniGoster(dataFilePath);            
            break;
        case 7:
            indexDosyasiniGoster(indexFilePath);
            break;
        case 8:
            indexDosyasiniSil(indexFilePath);
            break;
        default:
            printf("Gecersiz giris");
            break;
        }
    }

    return 0;
}

void indexOlustur(char *dataFilePath)
{
    FILE *dataFile;
    FILE *indexFile;
    int count = 0;
    int *datakeys;
    int *offsets;
    long int fileSize = 0;
    int recordCount = 0;

    struct kayit *ogr;
    int offset;

    size_t readElementCount;

    dataFile = fopen(dataFilePath, "rb");
    fseek(dataFile, 0, SEEK_END);
    fileSize = ftell(dataFile);
    recordCount = fileSize / sizeof(struct kayit);
    datakeys = malloc(fileSize);
    offsets = malloc(recordCount * sizeof(int));
    memset(datakeys, 0, fileSize);

    indexFile = fopen("index.txt", "w+");

    fseek(dataFile, 0, 0);

    while (ogr = KayitOku(dataFile))
    {
        offset = count * sizeof(struct kayit);
        datakeys[count] = ogr->ogrNo;
        offsets[count] = offset;

        count++;
    }
    
    SortKayit(datakeys, offsets, recordCount);

    for (count = 0; count < recordCount; count++)
    {   
        
        fprintf(indexFile, "%d\t%d\n", datakeys[count], offsets[count]);
        
    }

    fclose(dataFile);
    fclose(indexFile);
}

void SortKayit(int *kayitlar, int *offsets, int kayitSayisi)
{
    int i, j;
    int swapKayit;
    int swapOffset;

    for (i = 0; i < kayitSayisi - 1; i++)
    {
        for (j = i + 1; j < kayitSayisi; j++)
        {
            if (kayitlar[i] > kayitlar[j])
            {
                swapKayit = kayitlar[j];
                kayitlar[j] = kayitlar[i];
                kayitlar[i] = swapKayit;

                swapOffset = offsets[j];
                offsets[j] = offsets[i];
                offsets[i] = swapOffset;
            }
        }
    }
}

struct kayit *KayitOku(FILE *dataFile)
{

    void *buffer;
    struct kayit *ogr;
    size_t readElementCount;

    buffer = (void *)malloc(1 * sizeof(struct kayit));
    memset(buffer, 0, sizeof(struct kayit));

    readElementCount = fread(buffer, sizeof(struct kayit), 1, dataFile);

    if (readElementCount == 0)
    {
        free(buffer);

        return NULL;
    }

    ogr = (void *)malloc(1 * sizeof(struct kayit));
    *ogr = *((struct kayit *)buffer);

    free(buffer);

    return ogr;
}

void kayiEkle(char *dataFilePath, char *indexFilePath)
{
    FILE *indexFile;
    FILE *dataFile;
   
    indexFile = fopen(indexFilePath, "a+");
    dataFile = fopen(dataFilePath, "ab+");

    struct kayit o;

    printf("Ogrenci no giriniz:\n");
    scanf("%d", &(o.ogrNo));

    printf("Ders kodu giriniz:\n");
    scanf("%d", &(o.dersKodu));

    printf("Puan giriniz:\n");
    scanf("%d", &(o.puan));
    memset(&(o.ad), 0, sizeof(o.ad));
    printf("Ad giriniz:\n");
    scanf("%s", &(o.ad));

    memset(&(o.soyad), 0, sizeof(o.soyad));
    printf("Soyad giriniz:\n");
    scanf("%s", &(o.soyad));

    fwrite(&o, sizeof(struct kayit), 1, dataFile);

    fclose(dataFile);
    fclose(indexFile);

    indexOlustur("data.bin");
}

void indexDosyasiniGoster(char *indexFilePath)
{ 
    int ogrNo;
    int offset;
    
    FILE *indexDosyasi;
    indexDosyasi = fopen(indexFilePath, "r+");
    if (indexDosyasi == NULL)
    {
        printf("Dosya bulunamadi.");
        return;
    }

    fseek(indexDosyasi, 0, SEEK_SET);
    printf("Ogrenci listesi:\n");
    while (!feof(indexDosyasi))
    {

        fscanf(indexDosyasi, "%d\t%d\n", &ogrNo, &offset);
        
        printf("Ogrenci no: %d\toffset: %d\n", ogrNo, offset);
        
    }
    fclose(indexDosyasi);
}

void veriDosyasiniGoster(char *dataFilePath)
{
    // tamam
    FILE *dataFile;
    struct kayit *ogr;
    int count = 1;
    dataFile = fopen(dataFilePath, "rb");
    fseek(dataFile, 0, 0);

    while (ogr = KayitOku(dataFile))
    {   
        printf("%d. ogrenci\n", count);
        printf("Ogrenci no: %d\n", ogr->ogrNo);
        printf("Ders kodu: %d\n", ogr->dersKodu);
        printf("Puani: %d\n", ogr->puan);
        printf("Adi: %s\n", ogr->ad);
        printf("Soyadi: %s\n\n", ogr->soyad);
        count++;

    
    }

    fclose(dataFile);
}

void indexDosyasiniSil(char *fileName)
{
    remove(fileName);
    printf("Index dosyasi silindi.");
}

int kayitBul(char *dataFilePath, char *indexFilePath)
{
    FILE *dataFile;
    FILE *indexFile;
    struct kayit *ogr; 
    void *buffer; 
    int ogrNo; 
    int ogrNo1, offset1;
    int line=0;   
    int c = 0;
    int mid, last, first = 0;  
    dataFile = fopen(dataFilePath, "rb"); 
    indexFile = fopen(indexFilePath, "r");
    printf("Bulmak istediginiz ogrencinin numarasi: ");
    scanf("%d", &ogrNo); 
    fseek(indexFile, 0, 0); 

    while (!feof(indexFile)) 
    {
        if (fscanf(indexFile, "%d\t%d\n", &ogrNo1, &offset1))
        {
            line++;
        }
    }
    int No[line], offset[line];
    fseek(indexFile, 0, 0);
    while (!feof(indexFile))
    {
        fscanf(indexFile, "%d\t%d\n", &No[c], &offset[c]);
        c++;
    }
    last = line - 1;
    mid = (last + first) / 2;
    while (first <= last)
    {
        if (No[mid] < ogrNo)
            first = mid + 1;
        else if (No[mid] == ogrNo)
            break;
        else
            last = mid - 1;

        mid = (first + last) / 2;
    }

    int up = 0;
    while (No[mid - up] == ogrNo)
    {
        up++;
    }
    up=up-1;
    
    int tf;
    while (1)
    {
        if (No[mid - up] == ogrNo)
        {

            buffer = (void *)malloc(1 * sizeof(struct kayit));
            ogr = (void *)malloc(1 * sizeof(struct kayit));

            memset(buffer, 0, sizeof(struct kayit));

            fseek(dataFile, offset[mid - up], 0);
            fread(buffer, sizeof(struct kayit), 1, dataFile);
            *ogr = *((struct kayit *)buffer);

            printf("Aradiginiz ogrenci kaydi asagidiki mi?\n");
            printf("Ogrenci no: %d\n", ogr->ogrNo);
            printf("Ders kodu: %d\n", ogr->dersKodu);
            printf("Puani: %d\n", ogr->puan);
            printf("Adi: %s\n", ogr->ad);
            printf("Soyadi: %s\n", ogr->soyad);
            printf("Buysa 1 degilse 0 giriniz: ");
            scanf("%d", &tf);
        }
        
        else
        {
            printf("Aradiginiz kayit bulunmamaktadir.");
            free(ogr);
            free(buffer);
            return 1;
        }

        if(tf ==0){
            up--;
            continue;
        }
        else if(tf == 1){
            free(buffer);
            free(ogr);
            break;
        }
    }
    
    fclose(dataFile);
    fclose(indexFile);

    return offset[mid - up];
}

void kayitSil(char *dataFilePath, char *indexFilePath)
{
    FILE *dataFile;
    FILE *indexFile;
    void *buffer;

    buffer = (void *)malloc(1 * sizeof(struct kayit));
    dataFile = fopen(dataFilePath, "r+b");
    indexFile = fopen(indexFilePath, "r+");

    int offset, delete = 1;
    offset = kayitBul(dataFilePath, indexFilePath);
    if (offset == 1)
    {
        return;
    }

    fseek(dataFile, offset + (52 * delete), 0);

    while (1)
    {
        memset(buffer, 0, sizeof(struct kayit));
        if (fread(buffer, sizeof(struct kayit), 1, dataFile))
        {   
            fseek(dataFile, offset + ((delete -1 ) * 52), 0);
            fwrite(buffer, sizeof(struct kayit), 1, dataFile);
            fseek(dataFile, offset + (52 * (delete + 1)), 0);
            delete ++;
        }
        else{
            int size = offset + (52 * (delete - 1));
            truncate(dataFilePath, size);
             break;
        }
           
    }
    free(buffer);
    indexOlustur("data.bin");
    fclose(dataFile);
    fclose(indexFile);
}

void kayitGuncelle(char *dataFilePath, char *indexFilePath)
{
    FILE *dataFile;
    FILE *indexFile;
    int not, offset;

    dataFile = fopen(dataFilePath, "r+b");
    indexFile = fopen(indexFilePath, "r+");
    offset = kayitBul(dataFilePath, indexFilePath);
    if (offset == 1)
        return;
    printf("Ogrecinin yeni puanini giriniz: ");
    scanf("%d", &not );
    fseek(dataFile, offset + 8, 0);
    fwrite(&not, sizeof(int), 1, dataFile);
    printf("Ogrecinin yeni puani: %d\n", not );
    fclose(dataFile);
    fclose(indexFile);
}