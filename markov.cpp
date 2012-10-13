#include "markov.h"
#include "WAVio.h"

#include <stdlib.h>
#include <time.h>

using namespace std;

bool precedingMatch(vector<int>* preceding1, vector<int>* preceding2) {
  if (preceding1->size() != preceding2->size()) return false;

  int score = 0;
  for (int i = 0; i < preceding1->size(); i++) {
    score += abs(preceding1->at(i) - preceding2->at(i));
  }

  return score <= LENIENCY;
}



void initMarkovModel(vector<int>* samples, vector<progression*>* progs) {
  for (int i = 0; i < samples->size() - ORDER; i++) {
    vector<int>* sequence = new vector<int>;
    for (int j = i; j < i + ORDER; j++) {
      sequence->push_back(samples->at(j));
    }

    progression* p = NULL;

    for (int k = 0; k < progs->size(); k++) {
      if (precedingMatch(progs->at(k)->preceding, sequence)) {
        p = progs->at(k);
        break;
      }
    }

    if (p == NULL) {
      p = new progression;
      p->preceding = sequence;
      p->following = new vector<vector<int>* >;

      progs->push_back(p);
    }

    vector<int> *follow = new vector<int>;
    for (int l = i + ORDER; l < i + ORDER + FOLLOW; l++) {
      follow->push_back(samples->at(l));
    }

    p->following->push_back(follow);
  }
}

void generateSequence(vector<progression*>* progs, vector<int>& sequence) {
  int seedFreq = 0;

  srand( time(NULL) );

  for (int i = 0; i < progs->size(); i++) {
    progression* p = progs->at(i);

    if (p->following->size() > seedFreq) {
      seedFreq = p->following->size();

      sequence = *p->preceding;
    }
  }

  vector<int> seed = sequence;
  for (int i = 0; i < SIZE; i++) {
    for (int k = 0; k < progs->size(); k++) {
      if (precedingMatch(progs->at(k)->preceding, &seed)) {
        vector<vector<int>* >* following = progs->at(k)->following;
        vector<int>* follow = following->at(rand() % following->size());
        
        for (int l = 0; l < follow->size(); l++) {
          sequence.push_back(follow->at(l));

          seed.push_back(follow->at(l));
          seed.erase(seed.begin());
        }
        break;
      }
    }
  }
}

void debugProgs(vector<progression*>* progs) {

  for (int i = 0; i < progs->size(); i++) {
    progression* p = progs->at(i);
    cout << "Progression, preceding: ";
    for (int j = 0; j < p->preceding->size(); j++) {
      cout << p->preceding->at(j) << ",";
    }
    cout << endl << "Followed by:" << endl;
    for (int l = 0; l < p->following->size(); l++) {
      // print out individual follow
      vector<int>* follow = p->following->at(l);
      for (int m = 0; m < follow->size(); m++) {
        cout << follow->at(m) << ",";
      }
      cout << endl;
    }
  }

}

int markov ()
{
  vector<int> *samples = readSamplesFromWAV("audiosamples/Music/Dubstep 140.wav");
  // vector<int> *samples = new vector<int>;
  // samples->push_back(1);
  // samples->push_back(2);
  // samples->push_back(3);
  // samples->push_back(2);
  // samples->push_back(1);
  // samples->push_back(2);
  // samples->push_back(3);
  // samples->push_back(2);
  // samples->push_back(2);
  // samples->push_back(2);
  // samples->push_back(2);
  // samples->push_back(2);
  // samples->push_back(1);
  // samples->push_back(2);
  // samples->push_back(3);
  // samples->push_back(2);
  // samples->push_back(1);
  // samples->push_back(2);
  // samples->push_back(3);
  // samples->push_back(2);
  // samples->push_back(1);
  // samples->push_back(2);
  // samples->push_back(3);
  // samples->push_back(2);
  // samples->push_back(1);
  // samples->push_back(2);
  // samples->push_back(3);
  // samples->push_back(2);

  vector<progression*> *progs = new vector<progression*>;

  initMarkovModel(samples, progs);

  //debugProgs(progs);

  vector<int> sequence;
  generateSequence(progs, sequence);

  // cout << "Sequence: ";
  // for (int i = 0; i < sequence.size(); i++) {
  //   cout << sequence[i] << ",";
  // }
  // cout << endl;

  writeSamplesToWAV(&sequence, "output.wav");
  return 0;
}
