#include <fstream>
#include <ios>

#include "Dispatch.h"
#include "Decode.h"
#include "../Constants.h"

bool readNextMessage(std::ifstream& file)
{
	std::array<uint8_t, 2> lengthBuffer {};

	file.read(reinterpret_cast<char*>(lengthBuffer.data()), static_cast<std::streamsize>(lengthBuffer.size()));

	const auto bytesRead = file.gcount();

	if (bytesRead == 0 && file.eof()) { return false; }
	if (bytesRead != 2) { throw std::runtime_error("Truncated message length prefix"); }

	const auto length = convertBLEndian<std::uint16_t>(lengthBuffer.data());

	if (length == 0)
	{
		throw std::runtime_error("Invalid zero-length message");
	}

	if (length > Constants::kMaxMessageLength)
	{
		throw std::runtime_error("Unparsable: Message length greater than allowable max message length");
	}

	const auto msgLength = static_cast<std::size_t>(length);

	// Read the message body. The prefix length includes the message
	// type byte but does not include the two-byte prefix itself.
	file.read(reinterpret_cast<char*>(messageBuffer_.data()), static_cast<std::streamsize>(msgLength));

	if (file.gcount() != static_cast<std::streamsize>(msgLength))
	{
		throw std::runtime_error("Truncated message body");
	}

	return true;
}
