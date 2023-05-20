#include "empty_class.hpp"

namespace ProjectLibrary
{  // Metodo: Trovare metà del segmento
  Point PuntoMedio(Segment& segment,unsigned int new_id){
      double x_med=(segment.punto1.x+segment.punto2.x)*0.5;
      double y_med=(segment.punto1.y+segment.punto2.y)*0.5;
      unsigned int new_marker=segment.marker; // !=0 solo se segmento è già sul bordo
      Point puntomedio(x_med,y_med,new_id,new_marker);
      return puntomedio;}

  // Metodo: Dato lato trovare vertice opposto
  Point* VerticeOpposto(Cell& cella, Segment& lato){
      unsigned int i = 0;
      Point* punto = &cella.punti[i];
      while(punto->id==lato.punto1.id || punto->id==lato.punto2.id){
       i = i+1;
       punto = &cella.punti[i];
      }
      return punto;
  }

  // Metodo: Trova lato più lungo
  Segment* LatoMaggiore(Cell& cella){
    Segment* lm;
    double l1 = (cella.lati[1].lunghezza);
    double l0 = (cella.lati[0].lunghezza);
    double l2 = (cella.lati[2].lunghezza);
    if (l0>l1){
        if (l0>l2){lm=&cella.lati[0];}
            else{lm=&cella.lati[2];}}
    else{
        if (l1>l2){lm=&cella.lati[1];}
            else{lm=&cella.lati[2];}}
    return lm;
  }

  // Trova puntatore a vertice comune tra due lati, dando per scontato che ci sia
  Point* VerticeComune(Segment& lato1,Segment& lato2){
    Point* punto;
    if (lato1.punto1.id==lato2.punto1.id){punto=&lato1.punto1;}else{punto=&lato1.punto2;}
    return punto;
  }

  //Adiacenza (Meglio Lista di vettori)
  bool Adiacenti(Cell& cella1, Cell& cella2){
    bool flag = false;
    for(unsigned int i=0;i<3;i++){
     for(unsigned int j=0;j<3;j++){
        if(cella1.lati[i].id==cella2.lati[j].id){flag=true;}
     }
    }
    return flag;
  }

  void RiempiAdiacenti(Cell& cella, std::vector<Cell>& vettcelle){
  unsigned int n = vettcelle.size();
  for (unsigned int i=0;i<n;i++){
  if (Adiacenti(cella,vettcelle[i])){cella.adiac.push_back(&vettcelle[i]);}
  }
  }

  Cell* AdiacentiConLato(Cell& cella, Segment& lato, vector<Cell> triangoli){
    RiempiAdiacenti(cella,triangoli);
    Cell* it;
    for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            if((cella.adiac[i])->lati[j].id==lato.id && (cella.adiac[i])->id!=cella.id){
                it = &triangoli[i];
            }
        }
    }
    return it;
  }

// Dato ID trova punto dentro un dato vettore di punti (punto con id=ID esiste nel vettore) [INUTILIZZATA]
Point* TrovaPunto(std::vector<Point>& punti,unsigned int& id){
    unsigned int i = 0;
    Point* punto = &punti[i];
    while(punto->id!=id){
      i=i+1;
      punto = &punti[i];
    }
    return punto;
}

std::vector<Point> ImportPoints(std::string nomefile)
{   ifstream file;
    std::vector<Point> punti;
    string line;
    file.open(nomefile);
    getline(file,line);
    while (getline(file,line)){
        std::replace( line.begin(), line.end(),';',' ');
        istringstream conv(line);
        unsigned int id; // ID
        unsigned int marker; // Marker
        double x;
        double y;
        conv >>  id >> marker >> x >> y;
        Point punto(x,y,id,marker);
        punti.push_back(punto);
    }
    file.close();
    return punti;
}

std::vector<Segment> ImportSegments(std::string nomefile, std::vector<Point>& punti)
{   ifstream file;
    std::vector<Segment> lati;
    string line;
    file.open(nomefile);
    getline(file,line);
    while (getline(file,line)){
        std::replace( line.begin(), line.end(),';',' ');
        istringstream conv(line);
        unsigned int id; // ID
        unsigned int marker; // Marker
        unsigned int idOrigin;
        unsigned int idEnd;
        conv >>  id >> marker >> idOrigin >> idEnd;
        Point* puntoOrigin;
        Point* puntoEnd;
        for (unsigned int i=0;i<punti.size();i++){
            if (punti[i].id==idOrigin){puntoOrigin=&punti[i];}
            if (punti[i].id==idEnd){puntoEnd=&punti[i];}}
        Segment lato(*puntoOrigin,*puntoEnd,id,marker);
        lati.push_back(lato);
    }
    file.close();
    return lati;
}

std::vector<Cell> ImportCells(std::string nomefile, std::vector<Point>& punti, std::vector<Segment>& lati)
{ifstream file;
    std::vector<Cell> celle;
    string line;
    file.open(nomefile);
    getline(file,line);
    while (getline(file,line)){
        std::replace( line.begin(), line.end(),';',' ');
        istringstream conv(line);
        unsigned int id; // ID
        unsigned int idpunto1;
        unsigned int idpunto2;
        unsigned int idpunto3;
        unsigned int idlato1;
        unsigned int idlato2;
        unsigned int idlato3;
        conv >>  id >> idpunto1 >> idpunto2 >> idpunto3 >> idlato1 >> idlato2 >> idlato3;
        Point* punto1;
        Point* punto2;
        Point* punto3;
        Segment* lato1;
        Segment* lato2;
        Segment* lato3;
        for (unsigned int i=0;i<punti.size();i++){
            if (punti[i].id==idpunto1){punto1=&punti[i];}
            if (punti[i].id==idpunto2){punto2=&punti[i];}
            if (punti[i].id==idpunto3){punto3=&punti[i];}}
        for (unsigned int i=0;i<lati.size();i++){
            if (lati[i].id==idlato1){lato1=&lati[i];}
            if (lati[i].id==idlato2){lato2=&lati[i];}
            if (lati[i].id==idlato3){lato3=&lati[i];}}
        Cell cella(*punto1,*punto2,*punto3,*lato1,*lato2,*lato3,id);
        celle.push_back(cella);
    }
    file.close();
    return celle;
}

// Crea ID per Punto ricavato da due punti [INUTILIZZATA]
unsigned int PointIDGenerator(Point& punto1, Point& punto2){
    unsigned int id = stoul(to_string(punto1.id) + to_string(punto2.id)+"00");
    return id;
}

// Crea ID per Segmento ricavato da due punti [INUTILIZZATA]
unsigned int SegmentIDGenerator(Point& punto1, Point& punto2){
    unsigned int id = stoul(to_string(punto1.id) + to_string(punto2.id)+"00");
    return id;
}

// Crea ID per Celle ricavate da una Cella separata da un segmento [INUTILIZZATA]
std::vector<unsigned int> CellIDGenerator(Cell& cella, Segment& lato){
    std::vector<unsigned int> result;
    result.push_back(cella.id);
    unsigned int id = lato.id;
    result.push_back(id);
    return result;
}

std::vector<Segment> DividiLato(Segment& lato,unsigned int& new_index1,unsigned int& new_index2, Point& pmedio){
   Segment lato1;
   lato1.punto1 = lato.punto1;
   lato1.punto2 = pmedio;
   lato1.id=new_index1;
   lato1.marker=pmedio.marker;
   Segment lato2;
   lato1.punto1 = pmedio;
   lato1.punto2 = lato.punto2;
   lato1.id=new_index2;
   lato1.marker=pmedio.marker;
   std::vector<Segment> result;
   result.push_back(lato1);
   result.push_back(lato2);
   return result;
}

  void DividiCella(Cell& cella,
                   Segment& lato,
                   vector<Cell>& triangoli,
                   vector<Segment>& lati,
                   vector<Point>& punti,
                   unsigned int& maxIDP,
                   unsigned int& maxIDS,
                   unsigned int& maxIDC){
      Point puntomedio;
      puntomedio = PuntoMedio(lato,maxIDP);
      maxIDP=maxIDP+1;
      punti.push_back(puntomedio);
      unsigned int auxID = maxIDS+1; // ID secondo segmento generato
      vector<Segment> latigenerati = DividiLato(lato,maxIDS,auxID,puntomedio);
      maxIDS = maxIDS+2;
      lati.push_back(latigenerati[0]);
      lati.push_back(latigenerati[1]);
      Point* verticeopp;
      verticeopp = VerticeOpposto(cella,lato);
      if (lato.id==LatoMaggiore(cella)->id){
        Segment lato_bisez(*verticeopp,puntomedio,maxIDS);
        maxIDS = maxIDS + 1;
        lati.push_back(lato_bisez);
        // Faccio puntare l1 al lato che congiunge punto1 a VerticeOpposto e idem con l2 e punto2
        Segment* l1 = new Segment; // Puntatore a lato da punto1 di 'lato' a VerticeOpposto
        Segment* l2 = new Segment; // Puntatore a lato da punto2 di 'lato' a VerticeOpposto
        for(unsigned int j=0;j<3;j++){
            if(cella.lati[j].id!=lato.id){
                if(cella.lati[j].punto1.id==lato.punto1.id || cella.lati[j].punto2.id==lato.punto1.id){
                    l1 = &cella.lati[j];
                }
                else{
                    l2 = &cella.lati[j];}
            }
        }
        // Creo le due nuove celle generate dalla separazione di quella più grande
        Cell nuovacella1(lato.punto1,*verticeopp,puntomedio,lato_bisez,latigenerati[0],*l1,maxIDC);
        maxIDC = maxIDC+1;
        Cell nuovacella2(lato.punto2,*verticeopp,puntomedio,lato_bisez,latigenerati[1],*l2,maxIDC);
        maxIDC = maxIDC+1;
        triangoli.push_back(nuovacella1);
        triangoli.push_back(nuovacella2);
        //delete l1;
        //delete l2;
        unsigned int pos; // Indice della cella corrente nel vettore di celle
        for (unsigned int k=0;k<triangoli.size();k++){
            if(triangoli[k].id==cella.id){
                pos=k;
            }
        }
        triangoli.erase(triangoli.begin()+pos);
      }
      else{ // Se il lato passato in Input NON è il più lungo del triangolo
        Segment* latomax;
        latomax = LatoMaggiore(cella);
        Point puntomedioLM;
        puntomedioLM = PuntoMedio(*latomax,maxIDP);
        punti.push_back(puntomedioLM);
        maxIDP = maxIDP+1;
        Segment taglio(puntomedio,puntomedioLM,maxIDS);
        lati.push_back(taglio);
        maxIDS = maxIDS+1;
        Point* PuntoComune;
        PuntoComune = VerticeComune(lato,*latomax);
        auxID = maxIDS+1; // ID secondo segmento generato
        // Divido latomax trovato in due segmenti nel suo punto medio
        vector<Segment> latigeneratiLM = DividiLato(*latomax,maxIDS,auxID,puntomedioLM);
        maxIDS = maxIDS+2;
        lati.push_back(latigeneratiLM[0]);
        lati.push_back(latigeneratiLM[1]);
        Point* puntotaglio2; // Puntatore al Punto vertice del secondo taglio oltre a puntomedioLM

        //if(lato.punto1.id==PuntoComune->id){puntotaglio2=&lato.punto2;}else{puntotaglio2=&lato.punto1;}

        puntotaglio2=VerticeOpposto(cella,*latomax);
        Segment taglio2(puntomedioLM,*puntotaglio2,maxIDS);
        lati.push_back(taglio2);
        maxIDS = maxIDS+1;
        Segment* latopuntocomune = new Segment; // Lato generato da divisione di 'lato' dalla parte di PuntoComune
        Segment* latopuntotaglio2 = new Segment; // Lato generato da divisione di 'lato' dalla parte di puntotaglio2
        if (latigenerati[0].punto1.id==PuntoComune->id || latigenerati[0].punto2.id==PuntoComune->id){
            latopuntocomune = &latigenerati[0];
            latopuntotaglio2 = &latigenerati[1];
        }else{latopuntocomune = &latigenerati[1];
              latopuntotaglio2 = &latigenerati[0];}
        // delete latopuntocomune;
        // delete latopuntotaglio2;
        Segment* latoLMpuntocomune = new Segment; // Lato generato da divisione di 'latomax' dalla parte di PuntoComune
        Segment* latoLMverticeopp = new Segment; // Lato generato da divisione di 'latomax' dalla parte di verticeopp
        if (latigeneratiLM[0].punto1.id==PuntoComune->id || latigeneratiLM[0].punto2.id==PuntoComune->id){
            latoLMpuntocomune = &latigeneratiLM[0];
            latoLMverticeopp = &latigeneratiLM[1];
        }else{latoLMpuntocomune = &latigeneratiLM[1];
              latoLMverticeopp = &latigeneratiLM[0];}
        // delete latoLMpuntocomune;
        // delete latoLMverticeopp;
        Segment* terzolato = new Segment; // Individua il lato di 'cella' che non è 'latomax' nè 'lato'
        if (cella.lati[0].id!=lato.id && cella.lati[0].id!=latomax->id){
            terzolato=&cella.lati[0];
        }else{if (cella.lati[1].id!=lato.id && cella.lati[1].id!=latomax->id){
            terzolato=&cella.lati[1];
            }else{terzolato=&cella.lati[2];}
        }
        // delete terzolato;
        // Creo le tre nuove celle generate dalla separazione di quella più grande
        Cell nuovacella1(puntomedio,puntomedioLM,*PuntoComune,*latopuntocomune,*latoLMpuntocomune,taglio,maxIDC);
        maxIDC = maxIDC+1;
        Cell nuovacella2(puntomedio,puntomedioLM,*puntotaglio2,*latopuntotaglio2,taglio,taglio2,maxIDC);
        maxIDC = maxIDC+1;
        Cell nuovacella3(*puntotaglio2,*verticeopp,puntomedioLM,*terzolato,*latoLMverticeopp,taglio2,maxIDC);
        maxIDC = maxIDC+1;
        triangoli.push_back(nuovacella1);
        triangoli.push_back(nuovacella2);
        triangoli.push_back(nuovacella3);
        unsigned int pos; // Indice della cella corrente nel vettore di celle
        for (unsigned int k=0;k<triangoli.size();k++){
          if(triangoli[k].id==cella.id){
              pos=k;
          }
        }
        triangoli.erase(triangoli.begin()+pos);
  }
}
// Fine File
}
