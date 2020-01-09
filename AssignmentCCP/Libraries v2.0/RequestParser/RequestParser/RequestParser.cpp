#include <regex>
#include "Datastorage.h"
#include "RequestParser.h"

PostRequest::PostRequest() : valid(0)
{ }

PostRequest::~PostRequest()
{ }

PostRequest PostRequest::parse(std::string request)
{
	std::regex postRegex("^POST(@[^@#]*)#(.+)$");
	std::smatch postMatch;
	PostRequest post;

	if (!std::regex_match(request, postMatch, postRegex, std::regex_constants::match_default))
	{
		post.valid = 0;
		return post;
	}

	post.topicId = postMatch[1];
	post.message = postMatch[2];
	post.valid = 1;
	//todo: check if meant to be here
	dataUser.AddData(post.topicId, post.message, post.valid);
	return post;//TODO: pass as asyc??? ask about that
}

std::string PostRequest::getTopicId()
{
	return topicId;
}

std::string PostRequest::getMessage()
{
	return message;
}

std::string PostRequest::toString()
{
	return std::string("POST") + topicId + std::string("#") + message;
}

ReadRequest::ReadRequest() : valid(0)
{ }

ReadRequest::~ReadRequest()
{ }

ReadRequest ReadRequest::parse(std::string request)
{
	std::regex readRegex("^READ(@[^@#]*)#([0-9]+)$");
	std::smatch readMatch;
	ReadRequest read;

	if (!std::regex_match(request, readMatch, readRegex, std::regex_constants::match_default))
	{
		read.valid = 0;
		return read;
	}

	read.topicId = readMatch[1];
	read.postId = std::stoi(readMatch[2]);
	read.valid = 1;
	//todo: check if meant to be here
	dataUser.ShowData(read.topicId, read.postId, read.valid);
	return read;//TODO: pass as asyc??? ask about that
}

std::string ReadRequest::getTopicId()
{
	return topicId;
}

int ReadRequest::getPostId()
{
	return postId;
}

std::string ReadRequest::toString()
{
	return std::string("READ") + topicId + std::string("#") + std::to_string(postId);
}

CountRequest::CountRequest() : valid(0)
{ }

CountRequest::~CountRequest()
{ }

CountRequest CountRequest::parse(std::string request)
{
	std::regex countRegex("^COUNT(@[^@#]*)$");
	std::smatch countMatch;
	CountRequest count;

	if (!std::regex_match(request, countMatch, countRegex, std::regex_constants::match_default))
	{
		count.valid = 0;
		return count;
	}

	count.topicId = countMatch[1];
	count.valid = 1;
	//todo: check if meant to be here
	dataUser.CountData(count.topicId, count.valid);
	return count;//TODO: pass as asyc??? ask about that
}

std::string CountRequest::getTopicId()
{
	return topicId;
}

std::string CountRequest::toString()
{
	return std::string("COUNT") + topicId;
}

ListRequest::ListRequest() : valid(0)
{ }

ListRequest::~ListRequest()
{ }

ListRequest ListRequest::parse(std::string request)
{
	std::regex listRegex("^LIST$");
	std::smatch listMatch;
	ListRequest list;

	if (!std::regex_match(request, listMatch, listRegex, std::regex_constants::match_default))
	{
		list.valid = 0;
		return list;
	}

	list.valid = 1;

	return list;
}

std::string ListRequest::toString()
{
	return std::string("LIST");
}
