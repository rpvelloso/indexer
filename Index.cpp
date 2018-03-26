/*
 * Index.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include <sstream>
#include <algorithm>
#include <cmath>

#include "repos/Document.h"
#include "repos/Word.h"
#include "repos/WordCount.h"
#include "Index.h"

namespace idx {

static std::string comma = ",";
static std::string noop = "";

Index::Index(idx::DB &db) :
		db(db) {

};

Index::~Index() {

};

std::unordered_set<std::string> Index::stopWords = {
	"a", "ainda", "alem", "ambas", "ambos", "antes",
	"ao", "aonde", "aos", "apos", "aquele", "aqueles",
	"as", "assim", "com", "como", "contra", "contudo",
	"cuja", "cujas", "cujo", "cujos", "da", "das",
	"de", "dela", "dele", "deles", "demais", "depois",
	"desde", "desta", "deste", "dispoe", "dispoem", "diversa",
	"diversas", "diversos", "do", "dos", "durante", "e",
	"ela", "elas", "ele", "eles", "em", "entao",
	"entre", "essa ", "essas", "esse", "esses", "esta",
	"estas", "este", "estes", "ha", "isso", "isto",
	"logo", "mais", "mas", "mediante", "menos", "mesma",
	"mesmas", "mesmo", "mesmos", "na", "nas", "nao",
	"nas", "nem", "nesse", "neste", "nos", "o", "os",
	"ou", "outra", "outras", "outro", "outros", "pelas",
	"pelas", "pelo", "pelos", "perante", "pois", "por",
	"porque", "portanto", "proprio", "propios", "quais",
	"qual", "qualquer", "quando", "quanto  ", "que", "quem",
	"quer", "se", "seja", "sem", "sendo", "seu",
	"seus", "sob", "sobre", "sua", "suas", "tal",
	"tambem", "teu", "teus", "toda", "todas", "todo",
	"todos", "tua", "tuas", "tudo", "um", "uma",
	"umas", "uns", /**/ "para", "no", "i", "ii", "onde", "como"};

std::unordered_map<char, char> Index::accents = {
	{'à', 'a'}, {'á', 'a'},  {'â', 'a'},  {'ã', 'a'}, {'ä', 'a'},
	{'ç', 'c'},
	{'è', 'e'}, {'é', 'e'}, {'ê', 'e'}, {'ë', 'e'},
	{'ì', 'i'}, {'í', 'i'}, {'î', 'i'}, {'ï', 'i'},
	{'ñ', 'n'},
	{'ò', 'o'}, {'ó', 'o'}, {'ô', 'o'}, {'õ', 'o'}, {'ö', 'o'},
	{'ù', 'u'}, {'ú', 'u'}, {'û', 'u'}, {'ü', 'u'},
	{'ý', 'y'}, {'ÿ', 'y'},
	{'À', 'A'}, {'Á', 'A'}, {'Â', 'A'}, {'Ã', 'A'}, {'Ä', 'A'},
	{'Ç', 'C'},
	{'È', 'E'}, {'É', 'E'}, {'Ê', 'E'}, {'Ë', 'E'},
	{'Ì', 'I'}, {'Í', 'I'}, {'Î', 'I'}, {'Ï', 'I'},
	{'Ñ', 'N'},
	{'Ò', 'O'}, {'Ó', 'O'}, {'Ô', 'O'}, {'Õ', 'O'}, {'Ö', 'O'},
	{'Ù', 'U'}, {'Ú', 'U'}, {'Û', 'U'}, {'Ü', 'U'},
	{'Ý', 'Y'}};

bool Index::processWord(std::string &word) {
	try {
		stold(word);
		return false;
	} catch (...) {
		return stopWords.count(word) == 0;
	}
}

std::unordered_map<int, double> Index::queryVector(size_t n, const std::vector<int> &wordIdList) {
	std::unordered_map<int, double> qv;

	auto result = Word(db).list(wordIdList);
	for (auto &i:result) {
		int wid = i.first, df = i.second;
		qv[wid] = std::log((double)(n)/(double)(df));
	}

	return qv;
}

std::unordered_map<int, double> Index::documentVector(size_t n, int docId) {
	std::unordered_map<int, double> dv;

	auto result = WordCount(db).list(docId);

	int maxCount = std::numeric_limits<int>::min();

	for (auto &i:result) {
		int wid, count, df;

		wid = std::get<0>(i);
		count = std::get<1>(i);
		df = std::get<2>(i);

		if (count > maxCount)
			maxCount = count;

		dv[wid] = (double)(count) * std::log((double)(n) / (double)(df));
	}

	for (auto &i:dv)
		i.second /= (double)(maxCount);

	return dv;
}

void Index::preProcessString(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(),
	[](char a){
		if (accents.count(a) > 0)
			return accents[a];

		if (std::isalpha(a))
			return a;

		return ' ';
	});
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

double Index::cosineDistance(
	const std::unordered_map<int, double> &a,
	const std::unordered_map<int, double> &b) {

	auto pa = &a;
	auto pb = &b;

	if (a.size() > b.size())
		std::swap(pa, pb);

	double dotProduct = 0;
	double sizeA = 0, sizeB = 0;
	for (auto &i:*pa) {
		sizeA += std::pow(i.second, 2);
		auto j = pb->find(i.first);
		if (j != pb->end())
			dotProduct += i.second * j->second;
	}

	for (auto &i:*pb)
		sizeB += std::pow(i.second, 2);

	if (sizeA == 0 || sizeB == 0)
		return 0;

	return dotProduct / std::sqrt(sizeA * sizeB);
}

DB &Index::getDB() {
	return db;
}

std::string Index::getDocumentURI(int docId) {
	idx::Document doc(db, docId);
	return doc.getURI();
}

int Index::index(
	const std::string &uri, // uri = label
	std::string &document) {

	int docId = -1;
	preProcessString(document);

	std::unordered_map<std::string, size_t> wordCount;
	std::stringstream ss(document);
	std::string word;

	ss >> word;
	while (ss) {
		wordCount[word] += 1;
		ss >> word;
	}

	if (wordCount.size() > 0 && !uri.empty()) {
		idx::Document doc(db, uri);
		docId = doc.getID();
		//doc.clean();
		std::vector<std::tuple<int, int, int> > records;
		for (auto &wc:wordCount) {
			auto wordStr = wc.first;
			auto count = wc.second;

			if (processWord(wordStr)) {
				idx::Word word(db, wordStr);

				records.emplace_back(std::forward_as_tuple(doc.getID(), word.getID(), count));

				word.setDF(word.getDF()+1);
				word.update();
			}
		}

		if (records.size() > 0)
			idx::WordCount(db).insert(records);
	}

	return docId;
}

std::vector<std::pair<int, double> > Index::query(std::string &queryStr) {
	preProcessString(queryStr);

	std::stringstream ss(queryStr);
	std::string wordStr;
	std::vector<int> wordIdList;
	int n = idx::Document(db).count();

	ss >> wordStr;
	while (ss) {
		if (processWord(wordStr)) {
			idx::Word word(db, wordStr);
			wordIdList.emplace_back(word.getID());
		}

		ss >> wordStr;
	}

	auto qv = queryVector(n, wordIdList);

	auto result = WordCount(db).listDocuments(wordIdList);
	std::vector<std::pair<int, double> > resultSet;
	for (auto docId:result) {
		auto dv = documentVector(n, docId);
		auto score = cosineDistance(qv, dv);
		resultSet.emplace_back(std::make_pair(docId, score));
	}

	std::sort(resultSet.begin(), resultSet.end(),
		[](const std::pair<int, double> &a, const std::pair<int, double> &b){
			return a.second > b.second;
		});

	return resultSet;
}

} /* namespace idx */
