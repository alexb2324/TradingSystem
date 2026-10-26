#pragma once

#include <array>
#include <cstdio>

#include "../Usings.h"

class CountingHandler {

public:
	void onRawMessage(itch::Bytes message) noexcept
	{
		++counts_[message[0]];
		++total_;
		bytes_ += 2 + message.size();
	}

	void onBadMessage(itch::Bytes message) noexcept
	{
		++bad_;
	}

	void report(std::FILE* file) const
	{
		for (int t {}; t < 256; ++t)
		{
			if (counts_[t])
			{
				std::fprintf(file, "%c %12llu\n", t, static_cast<unsigned long long>(counts_[t]));
				std::fprintf(file, "total %llu	bytes %llu	bad %llu\n",
					static_cast<unsigned long long>(total_),
					static_cast<unsigned long long>(bytes_),
					static_cast<unsigned long long>(bad_));
			}
		}
	}

private:
	std::array<std::uint64_t, 256> counts_ {};
	std::uint64_t total_ {}, bytes_ {}, bad_ {};
};



