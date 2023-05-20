#include "empty_class.hpp"
using namespace ProjectLibrary;
int main()
{
    vector<Point> punti = ImportPoints("C:\\Users\\enric\\Desktop\\progetto_PCS\\Projects\\Raffinamento\\Dataset\\Test1\\Cell0Ds.csv");
  vector<Segment> lati = ImportSegments("C:\\Users\\enric\\Desktop\\progetto_PCS\\Projects\\Raffinamento\\Dataset\\Test1\\Cell1Ds.csv",punti);
  vector<Cell> triangoli = ImportCells("C:\\Users\\enric\\Desktop\\progetto_PCS\\Projects\\Raffinamento\\Dataset\\Test1\\Cell2Ds.csv",punti,lati);
  MergeSort(triangoli,0,triangoli.size()-1); // Ordino triangoli per area decrescente
  // Queste variabili tengono traccia dell'id minimo libero di ogni classe
  // in modo da fornire id validi (nuovi) per i nuovi elementi generati dal raffinamento
  unsigned int maxIDP = MaxID(punti)+1;
  unsigned int maxIDS = MaxID(lati)+1;
  unsigned int maxIDC = MaxID(triangoli)+1;

  cout<<"Area Cella Maggiore (Prima di Dividere): "<<triangoli[0].area<<endl;
  cout<<"Numero Celle (Prima di Dividere): "<<triangoli.size()<<endl;

  while (triangoli[0].area>0.1){
    DividiCella(triangoli[0],*LatoMaggiore(triangoli[0]),triangoli,lati,punti,maxIDP,maxIDS,maxIDC);
    MergeSort(triangoli,0,triangoli.size()-1); // Ordino triangoli per area decrescente
  }
  cout<<"Area Cella Maggiore (Dopo Divisione): "<<triangoli[0].area<<endl;
  cout<<"Numero Celle (Dopo Divisione): "<<triangoli.size()<<endl;

  return 0;
}

