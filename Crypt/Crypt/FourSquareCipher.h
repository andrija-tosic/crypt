﻿#pragma once
#include <array>
#include <filesystem>
#include <fstream>
#include <vector>


class FourSquareCipher
{
private:
	static std::vector<std::pair<char, char>> text_to_bigrams(std::string text) {
		std::vector<std::pair<char, char>> bigrams{};

		std::erase_if(text, isspace);

		for (size_t i = 0; i < text.length(); i += 2) {
			std::pair<char, char> chars{};
			chars.first = text[i];

			if (i + 1 < text.length()) {
				chars.second = text[i + 1];
			}
			else {
				chars.second = ' ';
			}

			bigrams.push_back(chars);
		}

		return bigrams;
	}

	static std::vector<std::pair<char, char>> encrypt_bigram(std::vector<std::pair<char, char>> bigram,
	                                                         const std::string_view& key_block1,
	                                                         const std::string_view& key_block2) {

		int i = 0;

		const std::string alphabet_block = "abcdefghiklmnopqrstuvwxyz";

		for (auto& [fst, snd] : bigram) {
			const size_t loc1 = alphabet_block.find(fst);
			const size_t loc2 = alphabet_block.find(snd);

			if (loc1 == std::string::npos) {
				bigram[i].first = fst;
			}
			else {
				bigram[i].first = key_block1[5 * (loc1 / 5) + loc2 % 5];
			}

			if (loc2 == std::string::npos) {
				bigram[i].second = snd;
			}
			else {
				bigram[i].second = key_block2[loc1 % 5 + 5 * (loc2 / 5)];
			}

			i++;
		}

		return bigram;
	}

	static std::vector<std::pair<char, char>> decrypt_bigram(std::vector<std::pair<char, char>> bigram,
	                                                         const std::string_view& key_block1,
	                                                         const std::string_view& key_block2) {
		int i = 0;

		const std::string alphabet_block = "abcdefghiklmnopqrstuvwxyz";

		for (auto& [fst, snd] : bigram) {
			const size_t loc1 = key_block1.find(fst);
			const size_t loc2 = key_block2.find(snd);

			if (loc1 == std::string::npos) {
				bigram[i].first = fst;
			}
			else {
				bigram[i].first = alphabet_block[5 * (loc1 / 5) + loc2 % 5];
			}

			if (loc2 == std::string::npos) {
				bigram[i].second = snd;
			}
			else {
				bigram[i].second = alphabet_block[loc1 % 5 + 5 * (loc2 / 5)];
			}

			i++;
		}

		return bigram;
	}

public:
	FourSquareCipher() = delete;
	~FourSquareCipher() = delete;
	FourSquareCipher(const FourSquareCipher&) = delete;
	FourSquareCipher(const FourSquareCipher&&) = delete;
	FourSquareCipher operator=(const FourSquareCipher&) = delete;
	FourSquareCipher operator=(const FourSquareCipher&&) = delete;


	static void encrypt(const std::string& file_path, const std::string& out_file_path) {
		std::basic_ifstream<char> file(file_path, std::ios::in);
		std::basic_ofstream<char> outfile(out_file_path, std::ios::out);

		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize file_size = file.gcount();
		file.clear();
		file.seekg(0, std::ios_base::beg);

		std::streamsize buf_size = std::min(static_cast<std::streamsize>(1024), file_size);
		auto text_buf = new char[buf_size];

		while (file.good()) {
			file.read(text_buf, buf_size);

			const std::string t(text_buf, text_buf + buf_size);

			std::vector<std::pair<char, char>> bigram = text_to_bigrams(t);
			std::vector<std::pair<char, char>> encrypted_bigram = encrypt_bigram(
				bigram, "zgptfoihmuwdrcnykeqaxvsbl", "mfnbdcrhsaxyogvituewlqzkp");

			std::string out_str{};
			for (auto& [fst, snd] : encrypted_bigram) {
				out_str += fst;
				out_str += snd;
			}

			outfile.write(out_str.c_str(), out_str.size());
		}

		delete[] text_buf;

		file.close();
		outfile.close();
	}

	static void decrypt(const std::string& file_path, const std::string& out_file_path) {
		std::basic_ifstream<char> file(file_path, std::ios::in);
		std::basic_ofstream<char> outfile(out_file_path, std::ios::out);

		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize file_size = file.gcount();
		file.clear();
		file.seekg(0, std::ios_base::beg);

		std::streamsize buf_size = std::min(static_cast<std::streamsize>(1024), file_size);
		auto text_buf = new char[buf_size];

		while (file.good()) {
			file.read(text_buf, buf_size);

			const std::string t(text_buf, text_buf + buf_size);

			std::vector<std::pair<char, char>> bigram = text_to_bigrams(t);
			std::vector<std::pair<char, char>> decrypted_bigram = decrypt_bigram(bigram, "zgptfoihmuwdrcnykeqaxvsbl",
				"mfnbdcrhsaxyogvituewlqzkp");

			std::string out_str{};
			for (auto& [fst, snd] : decrypted_bigram) {
				out_str += fst;
				out_str += snd;
			}

			outfile.write(out_str.c_str(), out_str.size());
		}

		delete[] text_buf;
		file.close();
		outfile.close();

	}
};
