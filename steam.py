
"""
EXAMPLE URL http://api.steampowered.com/ISteamUser/GetPlayerBans/v1/?key=4B3DF006EDCC1574630EB8F05B7CEE91&steamids=76561198379460868
https://steamcommunity.com/profiles/76561198080319355/
76561198379460868
"""
import json

import requests
import valve as valve
from discord.ext import commands
import valve

RUST_APP_ID = "252490"
PLAYER_BAN_URL = "http://api.steampowered.com/ISteamUser/GetPlayerBans/v1/?key=XXXX&steamids=YYYY"
USER_STATS_GAME_URL = "https://api.steampowered.com/ISteamUserStats/GetUserStatsForGame/v2/?key=XXXX&steamid=YYYY&appid=ZZZZ"
USER_NAME_URL = "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=XXXX&steamids=YYYY"
bot = commands.Bot(command_prefix='!')


def get_steam_ban(steamid='76561198080319355'):
    global PLAYER_BAN_URL
    apikey = ""
    with open('api.json') as apiserver:
        j_apiserver = json.load(apiserver)
        apikey = j_apiserver['apikey']
    apiquery = PLAYER_BAN_URL.replace("XXXX", apikey)
    apiquery = apiquery.replace("YYYY", steamid)
    response = requests.get(apiquery)
    values = "Account name : {0}\n".format(get_account_value(steamid, "personaname"))
    if response.status_code == 200:
        for r in response.json()['players']:
            for x in r:
                values += '{0} {1} {2}\n'.format(x, ':', r[x])
    else:
        values = "It was not possible to get a response from the main server. Error code {0}".\
            format(response.status_code)
    return values


def get_account_value(steamid, param):
    global USER_NAME_URL
    if steamid is None or steamid == "":
        print("No steam ID was provided.")
        return None
    else:
        apikey = ""
        with open('api.json') as apiserver:
            apikey = json.load(apiserver)['apikey']
        apiquery = USER_NAME_URL.replace("XXXX", apikey)
        apiquery = apiquery.replace("YYYY", steamid)
        response = requests.get(apiquery)
        param_value = ""
        if response.status_code == 200:
            for r in response.json()['response']['players']:
                for x in r:
                    if x == param:
                        param_value = r[x]
        else:
            param_value = "It was not possible to get a response from the main server. Error code {0}".\
                format(response.status_code)
        return param_value


def get_stats_game_user(steamid='76561198080319355'):
    global PLAYER_BAN_URL, RUST_APP_ID
    apikey = ""
    with open('api.json') as apiserver:
        j_apiserver = json.load(apiserver)
        apikey = j_apiserver['apikey']
    apiquery = USER_STATS_GAME_URL.replace("XXXX", apikey)
    apiquery = apiquery.replace("YYYY", steamid)
    apiquery = apiquery.replace("ZZZZ", RUST_APP_ID)
    print(apiquery)
    response = requests.get(apiquery)
    used_bullets = ""
    hitted_bullets = ""
    if response.status_code == 200:
        for r in response.json()['playerstats']['stats']:
            if r['name'] == "bullet_fired":
                used_bullets = r['value']
                print("Used bullets : {0}".format(r['value']))
            if r['name'] == "bullet_hit_player":
                hitted_bullets = r['value']
                print("Used bullets : {0}".format(r['value']))
        username = get_account_value(steamid, "personaname")
        msg = "Hit rate of player {0} (steamid is {1}) with guns : {2}%".format(username, steamid,
                                                                                round((int(hitted_bullets) /
                                                                                       int(used_bullets) * 100), 2))
    else:
        msg = "It was not possible to get a response from the main server. Error code {0}".format(response.status_code)
    print(msg)
    return msg


def get_stats_game_user_all(steamid='76561198080319355'):
    global PLAYER_BAN_URL, RUST_APP_ID
    apikey = ""
    with open('api.json') as apiserver:
        j_apiserver = json.load(apiserver)
        apikey = j_apiserver['apikey']
    apiquery = USER_STATS_GAME_URL.replace("XXXX", apikey)
    apiquery = apiquery.replace("YYYY", steamid)
    apiquery = apiquery.replace("ZZZZ", RUST_APP_ID)
    response = requests.get(apiquery)
    print(response.status_code)
    username = get_account_value(steamid, "personaname")
    all_stats = []
    stats = "Name of the account : {0}\n".format(username)
    chunk_size = 2000
    if response.status_code == 200:
        for r in response.json()['playerstats']['stats']:
            if len(stats) + len("{0} : {1}".format(r['name'], r['value'])) >= chunk_size:
                all_stats.append(stats)
                stats = "{0} : {1}".format(r['name'], r['value'])
            else:
                stats += "{0} : {1}\n".format(r['name'], r['value'])
        all_stats.append(stats)
    else:
        msg = "It was not possible to get a response from the main server. Error code {0}".format(response.status_code)
        all_stats.append(msg)
    print("Getting all stats of user {0} (steamid is {1})".format(username, steamid))
    print(len(stats))
    return all_stats


@bot.event
async def on_ready():
    for guild in bot.guilds:
        print(guild.name)
        if guild.name == "!IRULANDIA!":
            print(guild.name)
    print(f'{bot.user} is connected to Discord')


@bot.command(name='vac')
async def on_message(message):
    elements_message = message.message.content.lower().split(" ")
    if len(elements_message) == 2:
        steamid = elements_message[1]
        if message.author == bot.user:
            return
        elif message.message.content.lower() != "":
            stats = get_steam_ban(steamid)
            await message.channel.send(stats)
    else:
        if message.author == bot.user:
            return
        else:
            await message.channel.send("Wrong number of parameter for command stats. I need only one parameter.")


@bot.command(name='stats-all')
async def on_message(message):
    elements_message = message.message.content.lower().split(" ")
    if len(elements_message) == 2:
        steamid = elements_message[1]
        if message.author == bot.user:
            return
        elif message.message.content.lower() != "":
            stats = get_stats_game_user_all(steamid)
            for s in stats:
                await message.channel.send(s)
    else:
        if message.author == bot.user:
            return
        else:
            await message.channel.send("Wrong number of parameter for command stats. I need only one parameter.")


@bot.command(name='stats')
async def on_message(message):
    elements_message = message.message.content.lower().split(" ")
    if len(elements_message) == 2:
        steamid = elements_message[1]
        if message.author == bot.user:
            return
        elif message.message.content.lower() != "":
            stats = get_stats_game_user(steamid)
            await message.channel.send(stats)
    else:
        if message.author == bot.user:
            return
        else:
            await message.channel.send("Wrong number of parameter for command stats. I need only one parameter.")


def main():
    global bot, discord_guild
    token = ""
    discord_guild = ""
    with open('api.json') as apiserver:
        j_apiserver = json.load(apiserver)
        token = j_apiserver['discordkey']
        discord_guild = j_apiserver['discordguild']
    bot.run(token)


if __name__ == "__main__":
    main()
