/*
    Copyright 2018 Roberto Panerai Velloso.
    This file is part of indexer.
    Indexer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Indexer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Indexer.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

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
	"a", "agora", "ainda", "alem", "alguem", "algum", "alguma",
	"algumas", "alguns", "ambas", "ambos", "ampla", "amplas", "amplo",
	"amplos", "ante", "antes", "ao", "aonde", "aos", "apos", "aquela",
	"aquelas", "aquele", "aqueles", "aquilo", "as", "assim", "ate",
	"atraves", "cada", "coisa", "coisas", "com", "como", "contra",
	"contudo", "cuja", "cujas", "cujo", "cujos", "da", "daquele",
	"daqueles", "das", "de", "dela", "delas", "dele", "deles", "demais",
	"depois", "desde", "dessa", "dessas", "desse", "desses", "desta",
	"destas", "deste", "destes", "deve", "devem", "devendo", "dever",
	"devera", "deverao", "deveria", "deveriam", "devia", "deviam",
	"dispoe", "dispoem", "disse", "disso", "disto", "dito", "diversa",
	"diversas", "diversos", "diz", "dizem", "do", "dos", "durante",
	"e", "ela", "elas", "ele", "eles", "em", "enquanto", "entao",
	"entre", "era", "eram", "eramos", "essa", "essas", "esse",
	"esses", "esta", "estamos", "estao", "estas", "estava", "estavam",
	"estavamos", "este", "esteja", "estejam", "estejamos", "estes",
	"esteve", "estive", "estivemos", "estiver", "estivera", "estiveram",
	"estiveramos", "estiverem", "estivermos", "estivesse", "estivessem",
	"estivessemos", "estou", "eu", "fazendo", "fazer", "feita", "feitas",
	"feito", "feitos", "foi", "fomos", "for", "fora", "foram", "foramos",
	"forem", "formos", "fosse", "fossem", "fossemos", "fui", "grande",
	"grandes", "ha", "haja", "hajam", "hajamos", "hao", "havemos",
	"havia", "hei", "houve", "houvemos", "houver", "houvera", "houveram",
	"houveramos", "houverao", "houverei", "houverem", "houveremos", "houveria",
	"houveriam", "houveriamos", "houvermos", "houvesse", "houvessem",
	"houvessemos", "isso", "isto", "ja", "la", "lhe", "lhes", "lo", "logo",
	"mais", "mas", "me", "mediante", "menos", "mesma", "mesmas", "mesmo",
	"mesmos", "meu", "meus", "minha", "minhas", "muita", "muitas",
	"muito", "muitos", "na", "nao", "nas", "nem", "nenhum", "nessa",
	"nessas", "nesse", "nesta", "nestas", "neste", "ninguem", "no", "nos",
	"nossa", "nossas", "nosso", "nossos", "num", "numa", "nunca", "o",
	"os", "ou", "outra", "outras", "outro", "outros", "para", "pela",
	"pelas", "pelo", "pelos", "pequena", "pequenas", "pequeno", "pequenos",
	"per", "perante", "pode", "podendo", "poder", "poderia", "poderiam",
	"podia", "podiam", "pois", "por", "porem", "porque", "portanto", "posso",
	"pouca", "poucas", "pouco", "poucos", "primeiro", "primeiros", "propios",
	"propria", "proprias", "proprio", "proprios", "pude", "quais", "qual",
	"qualquer", "quando", "quanto", "quantos", "que", "quem", "quer",
	"sao", "se", "seja", "sejam", "sejamos", "sem", "sempre", "sendo",
	"ser", "sera", "serao", "serei", "seremos", "seria", "seriam", "seriamos",
	"seu", "seus", "si", "sido", "so", "sob", "sobre", "somos", "sou",
	"sua", "suas", "tal", "talvez", "tambem", "tampouco", "te", "tem",
	"temos", "tendo", "tenha", "tenham", "tenhamos", "tenho", "ter", "tera",
	"terao", "terei", "teremos", "teria", "teriam", "teriamos", "teu",
	"teus", "teve", "ti", "tido", "tinha", "tinham", "tinhamos", "tive",
	"tivemos", "tiver", "tivera", "tiveram", "tiveramos", "tiverem", "tivermos",
	"tivesse", "tivessem", "tivessemos", "toda", "todas", "todavia",
	"todo", "todos", "tu", "tua", "tuas", "tudo", "ultima", "ultimas", "ultimo",
	"ultimos", "um", "uma", "umas", "uns", "vendo", "ver", "vez", "vindo",
	"vir", "voce", "voces", "vos"};

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
