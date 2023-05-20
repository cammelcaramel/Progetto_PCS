#ifndef __EMPTY_H
#define __EMPTY_H
#include <Eigen/Eigen>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

namespace ProjectLibrary
{
  class Point{
    public:
    double x; // ascissa
    double y; // ordinata
    unsigned int id;
    unsigned int marker;

    Point(){x=0;y=0;id=0;marker=0;} // Costruttore Vuoto

    Point(double x,
          double y,
          unsigned int id,
          unsigned int marker): x(x),y(y),id(id),marker(marker){}

    Point(const Point& punto): x(punto.x),y(punto.y),id(punto.id),marker(punto.marker){}

    Point& operator=(const Point& punto) {
      if (this != &punto) {
        // Copia i valori dei membri
        x = punto.x;
        y = punto.y;
        id = punto.id;
        marker = punto.marker;
      }
      return *this;
    }
  };

  class Segment{
    public:
    Point punto1;
    Point punto2;
    unsigned int id;
    unsigned int marker;
    double lunghezza;
    static constexpr double TolleranzaLineare = 1.0e-12;

    Segment(Point& punto1,
            Point& punto2,
            unsigned int& id,
            unsigned int& marker): punto1(punto1),punto2(punto2),id(id),marker(marker),lunghezza(sqrt((abs(punto1.x-punto2.x))*(abs(punto1.x-punto2.x))+(abs(punto1.y-punto2.y))*(abs(punto1.y-punto2.y)))){}

    Segment(Point& punto1_,
            Point& punto2_,
            unsigned int& id_){ punto1=punto1_;
                                punto2=punto2_;
                                id=id_;
                                if(punto1_.marker==punto2_.marker){marker=punto1_.marker;}
                                   else{marker=0;}
                                lunghezza =sqrt((abs(punto1_.x-punto2_.x))*(abs(punto1_.x-punto2_.x))+(abs(punto1_.y-punto2_.y))*(abs(punto1_.y-punto2_.y)));
    }

    Segment(const Segment& lato){punto1=lato.punto1;punto2=lato.punto2;id=lato.id;marker=lato.marker;lunghezza=lato.lunghezza;}

    Segment(){
        Point punto1;
        Point punto2;
        id = 0;
        marker = 0;
        lunghezza = 0;
    }

    Segment& operator=(const Segment& lato) {
      if (this != &lato) {
        // Copia i valori dei membri
        punto1 = lato.punto1;
        punto2 = lato.punto2;
        id = lato.id;
        marker = lato.marker;
        lunghezza = lato.lunghezza;
      }
      return *this;
    }

    friend bool operator>(const Segment& lato1, const Segment& lato2) {
        return lato1.lunghezza > lato2.lunghezza + TolleranzaLineare;
      }

  };

  class Cell{
    public:
    unsigned int id;
    vector<Point> punti;
    vector<Segment> lati;
    vector<Cell*> adiac;
    double area;
    static constexpr double TolleranzaQuadratica = numeric_limits<double>::epsilon();

    Cell(Point& punto1,Point& punto2,Point& punto3, Segment& lato1, Segment& lato2, Segment& lato3,
         unsigned int& id_){
        punti.push_back(punto1);
        punti.push_back(punto2);
        punti.push_back(punto3);
        lati.push_back(lato1);
        lati.push_back(lato2);
        lati.push_back(lato3);
        id=id_;
        area= abs((punto3.x-punto1.x)*(punto2.y-punto1.y)-(punto3.y-punto1.y)*(punto2.x-punto1.x));
        Cell* adiac1 = nullptr; Cell* adiac2 = nullptr; Cell* adiac3 = nullptr;
        adiac.push_back(adiac1); adiac.push_back(adiac2); adiac.push_back(adiac3);
    }

    Cell(const Cell& cella){id=cella.id;punti=cella.punti;lati=cella.lati;adiac=cella.adiac;area=cella.area;} // Costruttore Copia

    Cell(){id = 0;
          Point punto1; Segment lato1; Cell* adiac1 = nullptr;
          Point punto2; Segment lato2; Cell* adiac2 = nullptr;
          Point punto3; Segment lato3; Cell* adiac3 = nullptr; area = 0;
          punti.push_back(punto1); lati.push_back(lato1); adiac.push_back(adiac1);
          punti.push_back(punto2); lati.push_back(lato2); adiac.push_back(adiac2);
          punti.push_back(punto3); lati.push_back(lato3); adiac.push_back(adiac3);}

    Cell& operator=(const Cell& cella) {
      if (this != &cella) {
        // Copia i valori dei membri
        punti = cella.punti;
        lati = cella.lati;
        id = cella.id;
        adiac = cella.adiac;
        area = cella.area;
      }
      return *this;
    }

    friend bool operator>(const Cell& cell1, const Cell& cell2) {
        return cell1.area > cell2.area + TolleranzaQuadratica;
      }

    };

  // Dati due lati trova vertice comune (Dando per scontato che lo abbiano)
  Point* VerticeComune(Segment& lato1,Segment& lato2);

  // Adiacenza (Meglio Lista di vettori) -> True se le celle sono adiacenti
  bool Adiacenti(Cell& cella1, Cell& cella2);

  // Data una cella e il vettore delle celle esistenti, riempie i suoi puntatori con le relative celle adiacenti
  void RiempiAdiacenti(Cell& cella, std::vector<Cell>& vettcelle);

  // Dato ID trova punto dentro un dato vettore di punti
  Point* TrovaPunto(std::vector<Point>& punti,unsigned int& id);

  // Metodo: Dato lato trovare vertice opposto
  Point* VerticeOpposto(Cell& cella, Segment& lato);

  // Metodo: Trovare metà del segmento
  Point PuntoMedio(Segment& segment,unsigned int new_id);

  // Metodo: Trova lato più lungo
  Segment* LatoMaggiore(Cell& cella);

  // Importa punti da file CSV in vettore di punti
  std::vector<Point> ImportPoints(std::string nomefile);

  // Importa segmenti da file CSV in vettore di segmenti
  std::vector<Segment> ImportSegments(std::string nomefile, std::vector<Point>& punti);

  // Importa celle da file CSV in vettore di celle
  std::vector<Cell> ImportCells(std::string nomefile, std::vector<Point>& punti, std::vector<Segment>& lati);

  // Crea ID per Punto ricavato da due punti
  unsigned int PointIDGenerator(Point& punto1, Point& punto2);

  // Crea ID per Segmento ricavato da due punti
  unsigned int SegmentIDGenerator(Point& punto1, Point& punto2);

  // Crea ID per Celle ricavate da una Cella separata da un segmento
  std::vector<unsigned int> CellIDGenerator(Cell& cella, Segment& lato);

  // Restituisce puntatore a Cella adiacente sul lato di input con la Cella di input
  Cell* AdiacentiConLato(Cell& cella, Segment& lato, vector<Cell> triangoli);

  // Dato un segmento restituisce i due segmenti in cui viene diviso dal punto medio
  std::vector<Segment> DividiLato(Segment& lato,unsigned int& new_index1,unsigned int& new_index2, Point& pmedio);

  // Merge per Area Celle
  template <typename T>
    void Merge(std::vector<T>& v,
               const unsigned int& sx,
               const unsigned int& cx,
               const unsigned int& dx){
  unsigned int i = sx; // Indice iterativo primo vettore (v fino a cx)
  unsigned int j = cx+1; // Indice iterativo secondo vettore (v dopo cx)
  unsigned int k = 0; // Indice vettore ordinato (ausiliario 'w')
  std::vector<T> w; // Vettore ausiliario in cui salvo elementi ordinati
  while ((i<=cx)&&(j<=dx)){
      if (v[i]>v[j]){
          w.push_back(v[i]); i=i+1;
      }else{
          w.push_back(v[j]); j=j+1;
      }
      k=k+1;
    }
  for (;i<=cx;i=i+1,k=k+1)w.push_back(v[i]); // Svuoto primo vettore
  for (;j<=dx;j=j+1,k=k+1)w.push_back(v[j]); // Svuoto secondo vettore
  for (i=sx;i<=dx;i=i+1) v[i] = w[i-sx]; // Ricopio da vettore ausiliario a originale
    }

    template <typename T>
    void MergeSort(std::vector<T>& v,const unsigned int& sx,const unsigned int& dx){
  if (sx < dx){
        unsigned int c = (sx+dx)/2;
        MergeSort(v,sx,c);
        MergeSort(v,c+1,dx);
        Merge(v,sx,c,dx);
    }
    }

 template <typename T>
  unsigned int MaxID(vector<T> vettore){
    unsigned int max = 0;
    for(unsigned int i=0;i<vettore.size();i++){
    if (vettore[i].id>max){max=vettore[i].id;}
    }
    return max;
  }

  template <typename T>
   T* Max_Geom(vector<T> vettore){
     T* max;
     for(unsigned int i=0;i<vettore.size();i++){
     if (vettore[i]>max){max=&vettore[i];}
     }
     return max;
   }

  void DividiCella(Cell& cella,
                   Segment& lato,
                   vector<Cell>& triangoli,
                   vector<Segment>& lati,
                   vector<Point>& punti,
                   unsigned int& maxIDP,
                   unsigned int& maxIDS,
                   unsigned int& maxIDC);
}
#endif // __EMPTY_H
