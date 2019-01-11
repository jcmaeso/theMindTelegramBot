#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>

typedef struct registeredChat {
	TgBot::Chat::Ptr chat;
	std::vector<TgBot::User::Ptr> users;
}registeredChat_t;

std::map<std::int64_t, std::shared_ptr<registeredChat_t>> chatList;



int main() {
	TgBot::Bot bot("790754969:AAHIKutqHMILadsGqSLJKajpBJKwirekZB0");
	bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Hi!");
	});
	bot.getEvents().onCommand("initialize", [&bot](TgBot::Message::Ptr message) {
		if (!chatList.count(message->chat->id)) {
			auto newChat = std::make_shared<registeredChat_t>();
			newChat->chat = message->chat;
			newChat->users.push_back(message->from);
			chatList[message->chat->id] = newChat;
			bot.getApi().sendMessage(message->chat->id, "A chat has been stored!");
		}
		else {
			bot.getApi().sendMessage(message->chat->id, "This chat has already been stored, don't use this cmd again");
		}
		
	});
	bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
		printf("User wrote %s\n", message->text.c_str());
		if (StringTools::startsWith(message->text, "/start")) {
			return;
		}
		bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
	});
	try {
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		TgBot::TgLongPoll longPoll(bot);
		while (true) {
			printf("Long poll started\n");
			longPoll.start();
		}
	}
	catch (TgBot::TgException& e) {
		printf("error: %s\n", e.what());
	}
	return 0;
}