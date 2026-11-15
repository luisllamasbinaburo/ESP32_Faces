// Tiny animated GIF encoder (indexed colour, LZW), enough to record the simulator output.
#ifndef SIM_GIFWRITER_h
#define SIM_GIFWRITER_h

#include <cstdio>
#include <cstdint>
#include <vector>
#include <unordered_map>

class GifWriter
{
public:
	// palette: up to 256 RGB888 colours. scale: integer pixel zoom
	bool Open(const char* path, int width, int height, int scale, const std::vector<uint32_t>& palette)
	{
		_f = fopen(path, "wb");
		if (!_f) return false;
		_w = width * scale; _h = height * scale; _scale = scale; _srcW = width;
		_bits = 1;
		while ((1 << _bits) < (int)palette.size()) _bits++;

		fwrite("GIF89a", 1, 6, _f);
		u16(_w); u16(_h);
		put(0xF0 | (_bits - 1)); put(0); put(0);
		for (int i = 0; i < (1 << _bits); i++)
		{
			uint32_t c = i < (int)palette.size() ? palette[i] : 0;
			put((c >> 16) & 0xFF); put((c >> 8) & 0xFF); put(c & 0xFF);
		}
		// Loop forever
		put(0x21); put(0xFF); put(11); fwrite("NETSCAPE2.0", 1, 11, _f); put(3); put(1); u16(0); put(0);
		return true;
	}

	// indices: width * height palette indices. delay in hundredths of a second
	void AddFrame(const std::vector<uint8_t>& indices, int delayCs)
	{
		put(0x21); put(0xF9); put(4); put(0x04); u16(delayCs); put(0); put(0);
		put(0x2C); u16(0); u16(0); u16(_w); u16(_h); put(0);

		std::vector<uint8_t> px(_w * _h);
		for (int y = 0; y < _h; y++)
			for (int x = 0; x < _w; x++)
				px[y * _w + x] = indices[(y / _scale) * _srcW + x / _scale];

		int minCode = _bits < 2 ? 2 : _bits;
		put(minCode);
		Lzw(px, minCode);
		put(0);
	}

	void Close()
	{
		if (!_f) return;
		put(0x3B);
		fclose(_f);
		_f = nullptr;
	}

	~GifWriter() { Close(); }

private:
	FILE* _f = nullptr;
	int _w = 0, _h = 0, _scale = 1, _srcW = 0, _bits = 1;

	std::vector<uint8_t> _block;
	uint32_t _acc = 0;
	int _accBits = 0;

	void put(int b) { fputc(b & 0xFF, _f); }
	void u16(int v) { put(v & 0xFF); put((v >> 8) & 0xFF); }

	void emit(int code, int size)
	{
		_acc |= (uint32_t)code << _accBits;
		_accBits += size;
		while (_accBits >= 8)
		{
			byte(_acc & 0xFF);
			_acc >>= 8;
			_accBits -= 8;
		}
	}

	void byte(uint8_t b)
	{
		_block.push_back(b);
		if (_block.size() == 255) flushBlock();
	}

	void flushBlock()
	{
		if (_block.empty()) return;
		put((int)_block.size());
		fwrite(_block.data(), 1, _block.size(), _f);
		_block.clear();
	}

	void Lzw(const std::vector<uint8_t>& px, int minCode)
	{
		const int clear = 1 << minCode, eoi = clear + 1;
		int codeSize = minCode + 1, next = eoi + 1;
		std::unordered_map<uint32_t, int> dict;

		emit(clear, codeSize);
		int prefix = px[0];
		for (size_t i = 1; i < px.size(); i++)
		{
			uint8_t c = px[i];
			uint32_t key = ((uint32_t)prefix << 8) | c;
			auto it = dict.find(key);
			if (it != dict.end()) { prefix = it->second; continue; }

			emit(prefix, codeSize);
			if (next < 4096)
			{
				dict[key] = next++;
				if (next > (1 << codeSize) && codeSize < 12) codeSize++;
			}
			else
			{
				emit(clear, codeSize);
				dict.clear();
				codeSize = minCode + 1;
				next = eoi + 1;
			}
			prefix = c;
		}
		emit(prefix, codeSize);
		emit(eoi, codeSize);
		if (_accBits > 0) { byte(_acc & 0xFF); _acc = 0; _accBits = 0; }
		flushBlock();
	}
};

#endif
