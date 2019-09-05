/* Random Bipartite Graph Generator
 * Input: 	m - the number of nodes in the left partition
 * 		n - the number of nodes in the right partition
 * 		mean - the average number of neighbors in the left partition of right partition nodes
 *		std - the standard deviation in the number of left neighbors of right partition nodes
 * 		oct - the number of nodes in the OCT set
 * 		octoct - the expected edge density within the oct set
 * 		octg - the expected edge density between the OCT set and the rest of the graph
 * 		seed - to seed the random graph generator, for replicability purposes
 *
 * Output: a bipartite graph with an OCT set, random, undirected, unweighted
 *
 * Biclique Project for ODE, U. of Tennessee and ORNL
 * Original Author: Yun Zhang
 *
 * Original code updated by Kyle Kloster and Andrew van der Poel to include functionality
 * of adding an OCT set to a bipartite graph
 *
 * Updated again by Hayley Russell to include using Barabasi-Albert model for edges within
 * the OCT set
 *
 * Originally Created: Oct 8, 2007
 * Last Updated: Apr 2019
 *
 *
 * Original version of this code by Yun Zhang, used under BSD 3-Clause license.
 * This version of the code modified from the original by adding the option to create an OCT set.
 * Modified by Kyle Kloster and Andrew van der Poel.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/*-----------------------------------------------------------------
 *  Function: box_muller()
 *    Implements the Polar form of the Box-Muller Transformation
 *    to generate a random variable with gaussian distribution
 *    of given mean and standard deviation
 *-----------------------------------------------------------------*/
float box_muller(float m, float s)	/* normal random variate generator */
{				                    /* mean m, standard deviation s */
  float x1, x2, w, y1;
  static float y2;
  static int use_last = 0;

  if (use_last) {		        /* use value from previous call */
    y1 = y2;
    use_last = 0;
  } else {
    do {
      x1 = 2.0 * drand48() - 1.0;
      x2 = 2.0 * drand48() - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;
    use_last = 1;
  }

  return( m + y1 * s );
}

/* -----------------------------------
 * Random bipartite graph generator
 * uses Barabasi-Albert model for edges within the octset
 * Function: output the edges in fly
 * ----------------------------------- */
void bigraph_generator_ba(FILE *fp, int m, int n, float mean, float std, int oct, float octoct, float octg, float octgcv, int seed)
{
  int e = 0;
  int degree[oct];
  memset( degree, 0, oct*sizeof(int) );
  float w, p, deg;

  // init random generator
  srand48(seed);

  // for every vertex v in V
  for (int v = 0; v < n; v++) {
	  // decide its degree using gaussian distribution with mean and variance
	  float d = box_muller(mean, std);
	  p = d / (float) m;  // edge density

    for (int u = 0; u < m; u++) {

      // output an edge with probability <= p
      w = drand48();
      if (w <= p) {
        e++;
        fprintf(fp, "%d\t%d\n", u, m+v);
      }
    }
  }

  int totalOctDegree = 0;
  // calculate oct degree stats
  float oct_mean, oct_std;
  oct_mean = (mean / (float) m) * (m + n);
  oct_std = octgcv * oct_mean;

  for (int o = 0; o < oct; o++) {
    // decide the number of nbrs it has in the bipartite portion
    deg = box_muller(oct_mean, oct_std);
    p = deg / (float) (m + n);

    //for every oct, non-oct vertex pair determine if they share an edge
    for (int i = 0; i < (m+n); i++) {
      // output an edge with probability <= p
      w = drand48();
      if (w <= p) {
        e++;
        fprintf(fp, "%d\t%d\n", i, o+m+n);
      }
    }
  }

  //seed for BA model, creates an s+1 clique
  // int s = 27;
  float exp = ((oct-1) / 2) * octoct;
  int s = (int)(exp+0.5);
  for (int i = 0; i < s+1; i++) {
    for (int j = i+1; j < s+1; j++) {
      e++;
      degree[i]++;
      degree[j]++;
      totalOctDegree+=2;
      fprintf(fp, "%d\t%d\n", i+m+n, j+m+n);
    }
  }

  //oct-oct edges
  //doesn't check for duplicate edges
  for (int o = s+1; o < oct; o++) {
    for (int i = 0; i < s; i++) {
      for (int j = 0; j < o; j++) {
        p = degree[j] / (float) totalOctDegree;
        w = drand48();
        if (w <= p) {
          e++;
          degree[j]++;
          degree[o]++;
          totalOctDegree += 2;
          fprintf(fp, "%d\t%d\n", j+m+n, o+m+n);
        }
      }
    }
    //for every added oct node, connect it to exactly s old oct nodes
    // for (int i = 0; i < s; i++) {
    //   w = drand48() * totalOctDegree;
    //   int index = 0;
    //   int sum = degree[index];
    //   while (w > sum) {
    //     index++;
    //     sum += degree[index];
    //   }
    //   e++;
    //   degree[o]++;
    //   degree[index]++;
    //   totalOctDegree += 2;
    //   fprintf(fp, "%d\t%d\n", index+m+n, o+m+n);
    // }
  }
  return;
}

int logtwo(int x) {
    int l = 0;
    x -= 1;
    while (x >= 1) {
        x >>= 1;
        l++;
    }
    return l;
}

void writebits(long* bitbuffer, int* used, int k, int value, FILE* fp) {
    *used += k;
    *bitbuffer = (*bitbuffer << k) + value;

    while (*used >= 6) {
        *used -= 6;
        char c = (*bitbuffer >> *used) + 63;
        fprintf(fp, "%c", c);
        *bitbuffer &= (1 << *used) - 1;
    }
}

/* -----------------------------------
 * Random bipartite graph generator
 * Function: output the edges in fly
 * ----------------------------------- */
void bigraph_generator(FILE *fp, FILE* octp, int m, int n, float mean, float std, int oct, float octoct, float octg, float octgcv, int seed)
{
  int u, v, o, i, j, e=0;
  float w, d, p, density, deg, prob;

  // init random generator
  srand48(seed);

  int prev = 0;
  long bitbuffer = 0;
  int used = 0;
  int tot = m + n + oct;
  int k = logtwo(tot);

  fprintf(fp, "%c", ':');

  if (tot <= 62) {
      fprintf(fp, "%c", tot + 63);
  }
  else if (tot <= 258047) {
      fprintf(fp, "%c", 126);
      int t;
      for (t = 12; t >= 0; t -= 6) {
          char c = ((tot >> t) & ((1 << 6) - 1)) + 63;
          fprintf(fp, "%c", c);
      }
  }
  else {
      fprintf(fp, "%c", 126);
      fprintf(fp, "%c", 126);
      int t;
      for (t = 30; t >= 0; t -= 6) {
          char c = ((tot >> t) & ((1 << 6) - 1)) + 63;
          fprintf(fp, "%c", c);
      }
  }

  // for every vertex v in V
  for (v = 0; v < n; v++) {
	  // decide its degree using gaussian distribution with mean and variance
	  d = box_muller(mean, std);
	  p = d / (float) m;  // edge density

    for (u = 0; u < m; u++) {
      // output an edge with probability <= p
      w = drand48();
      if (w <= p) {
        e++;
        // fprintf(fp, "%d\t%d\n", u, m+v);
        if (m+v == prev + 1) {
            writebits(&bitbuffer, &used, 1, 1, fp);
        }
        else if (m + v > prev) {
            writebits(&bitbuffer, &used, 1, 1, fp);
            writebits(&bitbuffer, &used, k, m + v, fp);
            writebits(&bitbuffer, &used, 1, 0, fp);
        }
        else {
            writebits(&bitbuffer, &used, 1, 0, fp);
        }

        writebits(&bitbuffer, &used, k, u, fp);
        prev = m + v;
      }
    }
  }

  // calculate oct degree stats
  float oct_mean, oct_std;
  oct_mean = octg * (m + n);
  oct_std = octgcv * oct_mean;

  if (octgcv > 1) {
      oct_mean -= oct_std * .3187 / .7475 / 2;
  }

  for (o = 0; o < oct; o++) {
	  // decide the number of nbrs it has in the bipartite portion
	  deg = box_muller(oct_mean, oct_std);
	  p = deg / (float) (m + n);

	  //for every oct, non-oct vertex pair determine if they share an edge
	  for (i = 0; i < (m+n); i++) {
	    // output an edge with probability <= p
	    w = drand48();
	    if (w <= p) {
		    e++;
		    // fprintf(fp, "%d\t%d\n", i, o+m+n);
            if (m+n+o == prev + 1) {
                writebits(&bitbuffer, &used, 1, 1, fp);
            }
            else if (m + n + o > prev) {
                writebits(&bitbuffer, &used, 1, 1, fp);
                writebits(&bitbuffer, &used, k, m + n + o, fp);
                writebits(&bitbuffer, &used, 1, 0, fp);
            }
            else {
                writebits(&bitbuffer, &used, 1, 0, fp);
            }

            writebits(&bitbuffer, &used, k, i, fp);
            prev = m + n + o;
	    }
	  }

	  //for every oct, oct vertex pair determine if they share an edge
	  for (j = 0; j < o; j++) {
	    w = drand48();
	    if (w <= octoct) {
		    e++;
		    // fprintf(fp, "%d\t%d\n", j+m+n, o+m+n);
            if (m+n+o == prev + 1) {
                writebits(&bitbuffer, &used, 1, 1, fp);
            }
            else if (m + n + o > prev) {
                writebits(&bitbuffer, &used, 1, 1, fp);
                writebits(&bitbuffer, &used, k, m + n + o, fp);
                writebits(&bitbuffer, &used, 1, 0, fp);
            }
            else {
                writebits(&bitbuffer, &used, 1, 0, fp);
            }

            writebits(&bitbuffer, &used, k, m + n + j, fp);
            prev = m + n + o;
	    }
	  }
  }

  writebits(&bitbuffer, &used, 6 - (used % 6), 0, fp);

  for (o = 0; o < oct; o++) {
      fprintf(octp, "%d\n", o+m+n);
  }

  printf("%d\n", e);

  return;
}


/* -----------------------------------
 * Driver
 * ----------------------------------- */
int main(int argc, char **argv)
{

  // initialize variables
  int n, m, oct;
  float mean, std, octoct, octg, octgcv;
  int seed;
  char name[100];
  char octname[100];

  if (argc < 8) {
    fprintf(stderr, "Usage: %s m n mean std seed\n", argv[0]);
	  fprintf(stderr, "  m - the expected number of nodes on one (left) side\n");
  	fprintf(stderr, "  n - the expected number of nodes on the other (right) side\n");
	  fprintf(stderr, "  mean - the mean of degree for right side vertices\n");
	  fprintf(stderr, "  std - the standard deviation of degree for right side vertices\n");
  	fprintf(stderr, "  oct - the # of oct nodes\n");
  	fprintf(stderr, "  oct-oct - the density within the oct set\n");
  	fprintf(stderr, "  oct-G- the density between the oct set and bipartite graph\n");
  	fprintf(stderr, "  the cv of the # of neighbors oct nodes have in the left and right side\n");
  	fprintf(stderr, "  seed - the seed\n");
  	fprintf(stderr, "\n");
    exit(1);
  }

  // get arguments
  m = atoi(argv[1]);  // number of left nodes
  n = atoi(argv[2]);  // number of right nodes
  mean = atof(argv[3]);	// mean of degree for right side
  std = atof(argv[4]);	// standard deviation of degree for right side
  oct = atoi(argv[5]);	// number of oct nodes
  octoct = atof(argv[6]);	//density within the oct set
  octg = atof(argv[7]);		//density between the oct set and the rest of the graph
  octgcv = atof(argv[8]);	//coefficient of variation of the # of neighbors oct nodes have in rest of graph
  seed = atof(argv[9]);
  strcpy(name, argv[10]);
  strcat(name, ".s6");
  strcpy(octname, argv[10]);
  strcat(octname, ".oct");

  FILE* g = fopen(name, "w");
  FILE* o = fopen(octname, "w");

  // generate a random bigraph with m,n,p
  bigraph_generator(g, o, m, n, mean, std, oct, octoct, octg, octgcv, seed);
  // bigraph_generator_ba(stdout, m, n, mean, std, oct, octoct, octg, octgcv, seed);

  exit(0);
}
