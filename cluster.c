/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>	//pouzitt funkci strchr()

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, args) printf(" - "__FILE__":%u: "s"\n",__LINE__,args)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
	int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
	c->size=0;
	c->obj=(struct obj_t *)malloc(sizeof(struct obj_t)*cap);
	c->capacity=(c->obj?cap:0);
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
	assert(c);

	if (c->obj) {
		free(c->obj);
		c->obj=NULL;
	}
	c->size=0;
	c->capacity=0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
	assert(c);

	if ( c->size==c->capacity ) {
		c=resize_cluster(c,c->capacity+CLUSTER_CHUNK);
		assert(c);
		assert(c->size<c->capacity);	// ujist?te se, ?e je m?sto pro nov? objekt
	}
	c->obj[c->size]=obj;
	c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
	for ( int i=0 ; i<c2->size ; i++ )
		append_cluster(c1,c2->obj[i]);
	sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    //Pridam jeste jednu nezbytnou kontrolu. A pak projdou 2 vyse
    //vlastni zaporne hodnoty idx
	assert(idx>=0);

	narr--;
    //presunout posledni shluk v poli na misto shluku, ktery se odstranuje
	carr[idx]=carr[narr];
	return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
	float xdiff=o1->x-o2->x;
	float ydiff=o1->y-o2->y;
	return sqrt(xdiff*xdiff+ydiff*ydiff);
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
	float dist=INT_MAX;
    //Iterujeme pres vsechny mozne dvojice objektu (kde jeden objekt v paru je z prvniho
    //shluky, druhy - od druheho) a najdete minimalni vzdalenost mezi temito pary.
    //Indexy objektu,nemusime pamatovat
	for ( int i=0 ; i<c1->size ; i++ )
		for ( int j=0 ; j<c2->size ; j++ ) {
			float d=obj_distance(&c1->obj[i],&c2->obj[j]);
			if ( d<dist )
				dist=d;
		}
//	assert(dist!=INT_MAX);
	return dist;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    assert(narr > 1);

	float mindist=INT_MAX;

    //Iterujeme vsechny mozne dvojice shluku a vypocitejte vzdalenosti ve dvojicich,
    //vysledkem je definovani dvojice shluku s minimalni vzdalenosti mezi nimi
	for ( int i=0 ; i<narr-1 ; i++ )
		for ( int j=i+1 ; j<narr ; j++ ) {
			float dist=cluster_distance(&carr[i],&carr[j]);
			if ( dist<mindist ) {
				mindist=dist;
				*c1=i;
				*c2=j;
			}
		}
    //V kazdem kroku shlukovani se hodnota nesmi snizit oproti
    //posledni krok, protoze slucovani shluku jde od nejblizsich k dalsim a dalsim
    // vzdalenejsi od sebe

}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
	*arr=NULL;

	FILE *f=fopen(filename,"r");
	if ( f==NULL ) {
		printf("Error: can't load data file.\n"); //chyba souboru
		return 0;
	}

	const int buffsize=50;	//proste takhle 50
	char buff[buffsize];

    //precist pocet objektu v souboru
	char *s=fgets(buff,buffsize,f);
	if ( s==NULL ) {
		printf("Error: can't load first line from data file.\n");//chyba na ??dku v souboru
		return 0;
	}
	char *p=strchr(buff,'=');	//hledani znaku '=' v retezci
	if ( p==NULL ) {
		printf("Error: wrong format of line with objects count.\n");//chyba radku
		return 0;
	}
	int cnt=atoi(p+1);	//pokousim se prevest na cislo tu cast retezce, ktera je za znakem '='
	if ( cnt<=0 ) {
		printf("Error: wrong value of objects count.\n");//pocet objektu chyba
		return 0;
	}

    //Pote, co jsme zjistili pocet objektu, alokujeme pamet pro celou jejich sadu
	*arr=(struct cluster_t *)malloc(sizeof(struct cluster_t)*cnt);
	assert(*arr);

    //Cteni objektu
	for ( int i=0 ; i<cnt ; i++ ) {
		s=fgets(buff,buffsize,f);
		if ( s==NULL ) {
			printf("Error: data file nas now line for object N%i.\n",i+1);//v datovem souboru chybi radek pro objekt
			free(*arr);
			*arr=NULL;
			return 0;
		}
		int id, x, y;
		int n=sscanf(s,"%i %i %i",&id,&x,&y);
		if ( n!=3 ) {
			printf("Error: wrong number of values in the line %i of data file.\n",i+1);
            //nespravni pocet hodnot v retezci
			free(*arr);
			*arr=NULL;
			return 0;
		}
		if ( id<=0 || x<0 || y<0 ) {
			printf("Error: wrong values in line %i of data file.\n",i+1);
			//nespravni hodnoty v retezci
			free(*arr);
			*arr=NULL;
			return 0;
		}
		init_cluster(&(*arr)[i],1);
		(*arr)[i].size=1;
		(*arr)[i].obj[0].id=id;
		(*arr)[i].obj[0].x=x;
		(*arr)[i].obj[0].y=y;
	}

	fclose(f);
	return cnt;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
	if ( argc<2 ) {
		printf("Error: no data file is specified.\n"); //soubor neni zadan
		return 1;	//kod chyby, na rozdil od 0, kdyz program normalne konci
	}

	int numofobjects=load_clusters(argv[1],&clusters);
	if ( numofobjects==0 )
		return 1;

	int numofc=1;	//zakladni hodnota
	if ( argc==3 ) {
        // prevzit cilovz pocet clusteru z prikazoveho radku.
		numofc=atoi(argv[2]);
		if ( numofc<1 ) {
			printf("Error: wrong number of clusters is specified.\n");//chyba cisla clusteru
			return 1;
		}
		if ( numofc>numofobjects ) {
			printf("Error: specified number of clusters is greater than number of objects.\n");
			//pocet clusteru je vyssi nez hodnoty
			return 1;
		}
	}




    //Provedeme shlukovani, pocatecni pocet shluku se rovna poctu objektu.
	int currnumofc=numofobjects;
    //zatimco aktualni pocet clusteru je vetsi nez cilovy - postupne sloucit
    //pary nejblizsich shluku
	while ( currnumofc>numofc ) {
		int c1,c2;
		find_neighbours(clusters,currnumofc,&c1,&c2);
        //kontrola spravnosti volane funkce - kontrola hodnot
        //datove hodnoty, ktere jsou z nej prijimany
		assert(c1>=0);
		assert(c2>=0);
		assert(c1!=c2);
		merge_clusters(&clusters[c1],&clusters[c2]);
		currnumofc=remove_cluster(clusters,currnumofc,c2);
	}

    // vystup konecneho vysledku
	printf("Clustering %i data objects onto %i clusters.\n",numofobjects,numofc);
	print_clusters(clusters,numofc);

    // Odstraneni objektu a shluku. Protoze pri slucovani shluku (ve funkci merge_clusters)
    // pridany cluster neni  smazan, je nutne projit vsechny existujici clustery.
    //clusters. A jejich pocatecni pocet se rovnal poctu objektu nactenych ze souboru.
	for ( int i=0 ; i<numofobjects ; i++ )
		clear_cluster(&clusters[i]);
	free(clusters);
	return 0;
}
