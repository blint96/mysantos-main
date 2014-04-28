-- phpMyAdmin SQL Dump
-- version 3.3.7deb7
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Czas wygenerowania: 22 Mar 2014, 23:24
-- Wersja serwera: 5.1.73
-- Wersja PHP: 5.3.3-7+squeeze18

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Baza danych: `testowy`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla  `areas`
--

CREATE TABLE IF NOT EXISTS `areas` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `owner` int(11) NOT NULL DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '0',
  `minx` float NOT NULL DEFAULT '0',
  `miny` float NOT NULL DEFAULT '0',
  `maxx` float NOT NULL DEFAULT '0',
  `maxy` float NOT NULL DEFAULT '0',
  `name` varchar(64) NOT NULL DEFAULT '0',
  `objects` int(11) NOT NULL DEFAULT '0',
  `color` int(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `areas`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `bankomaty`
--

CREATE TABLE IF NOT EXISTS `bankomaty` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `vw` int(11) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `bankomaty`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `banned_ip`
--

CREATE TABLE IF NOT EXISTS `banned_ip` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `banned_ip`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `bustops`
--

CREATE TABLE IF NOT EXISTS `bustops` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `name` varchar(64) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `bustops`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `char_opisy`
--

CREATE TABLE IF NOT EXISTS `char_opisy` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` int(11) NOT NULL,
  `tresc` varchar(128) NOT NULL,
  `title` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `char_opisy`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `core_attachments`
--

CREATE TABLE IF NOT EXISTS `core_attachments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `item_uid` int(11) NOT NULL,
  `bone` int(11) NOT NULL DEFAULT '2',
  `posx` float NOT NULL,
  `posy` float NOT NULL,
  `posz` float NOT NULL,
  `rotx` float NOT NULL,
  `roty` float NOT NULL,
  `rotz` float NOT NULL,
  `scalex` float NOT NULL,
  `scaley` float NOT NULL,
  `scalez` float NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `core_attachments`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `core_items`
--

CREATE TABLE IF NOT EXISTS `core_items` (
  `item_uid` int(11) NOT NULL AUTO_INCREMENT,
  `item_owneruid` int(11) NOT NULL,
  `item_name` varchar(32) NOT NULL,
  `item_value1` int(11) NOT NULL,
  `item_value2` int(11) NOT NULL,
  `item_stacjafm` varchar(128) NOT NULL,
  `item_type` int(11) NOT NULL,
  `item_posx` float NOT NULL,
  `item_posy` float NOT NULL,
  `item_posz` float NOT NULL,
  `item_objectid` int(11) NOT NULL,
  `item_whereis` int(3) NOT NULL,
  `item_used` int(3) NOT NULL,
  `item_worldid` int(11) NOT NULL,
  PRIMARY KEY (`item_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `core_items`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `core_objects`
--

CREATE TABLE IF NOT EXISTS `core_objects` (
  `object_uid` int(11) NOT NULL AUTO_INCREMENT,
  `object_ownertype` int(11) NOT NULL,
  `object_ownerid` int(11) NOT NULL,
  `object_model` int(11) NOT NULL,
  `object_posx` float NOT NULL,
  `object_posy` float NOT NULL,
  `object_posz` float NOT NULL,
  `ox` float NOT NULL,
  `oy` float NOT NULL,
  `oz` float NOT NULL,
  `object_rotatex` float NOT NULL,
  `object_rotetey` float NOT NULL,
  `object_rotatez` float NOT NULL,
  `object_world` int(11) NOT NULL,
  `object_interior` int(11) NOT NULL,
  `object_type` int(11) NOT NULL,
  PRIMARY KEY (`object_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `core_objects`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `core_plants`
--

CREATE TABLE IF NOT EXISTS `core_plants` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `vw` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `core_plants`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `core_players`
--

CREATE TABLE IF NOT EXISTS `core_players` (
  `char_uid` int(11) NOT NULL AUTO_INCREMENT,
  `char_name` varchar(32) NOT NULL,
  `sex` int(5) NOT NULL,
  `char_password` varchar(32) NOT NULL DEFAULT 'brak',
  `char_time` int(24) NOT NULL DEFAULT '0',
  `char_money` int(11) NOT NULL DEFAULT '1000',
  `char_bank` int(24) NOT NULL DEFAULT '0',
  `char_banknr` int(10) NOT NULL DEFAULT '0',
  `char_warns` int(5) NOT NULL DEFAULT '0',
  `char_gamescore` int(11) NOT NULL DEFAULT '0',
  `char_wiek` int(11) NOT NULL,
  `char_health` float NOT NULL DEFAULT '100',
  `char_posx` float NOT NULL DEFAULT '0',
  `char_posy` float NOT NULL DEFAULT '0',
  `char_posz` float NOT NULL DEFAULT '0',
  `char_angle` float NOT NULL DEFAULT '0',
  `char_logged` int(3) NOT NULL DEFAULT '0',
  `char_gid` int(5) NOT NULL,
  `char_skin` int(5) NOT NULL,
  `char_bwtime` int(5) NOT NULL DEFAULT '0',
  `char_ajtime` int(5) NOT NULL DEFAULT '0',
  `block_char` int(1) NOT NULL DEFAULT '0',
  `block_ooc` int(1) NOT NULL DEFAULT '0',
  `block_vehs` int(1) NOT NULL DEFAULT '0',
  `block_ban` int(1) NOT NULL DEFAULT '0',
  `ban_reason` varchar(64) NOT NULL DEFAULT 'brak',
  `dowod` int(1) NOT NULL DEFAULT '1',
  `prawkoa` int(1) NOT NULL DEFAULT '0',
  `prawkob` int(1) NOT NULL DEFAULT '0',
  `prawkoc` int(1) NOT NULL DEFAULT '0',
  `prawkoce` int(1) NOT NULL DEFAULT '0',
  `prawkod` int(1) NOT NULL DEFAULT '0',
  `char_world` int(11) NOT NULL DEFAULT '0',
  `char_interior` int(11) NOT NULL DEFAULT '0',
  `char_spawn` int(2) NOT NULL DEFAULT '0',
  `char_kondycja` float NOT NULL DEFAULT '10',
  `char_sila` float NOT NULL DEFAULT '10',
  `char_fs` int(5) NOT NULL DEFAULT '4',
  `char_shootskill` float NOT NULL DEFAULT '10',
  PRIMARY KEY (`char_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `core_players`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `corners`
--

CREATE TABLE IF NOT EXISTS `corners` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `owner` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `corners`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `doors`
--

CREATE TABLE IF NOT EXISTS `doors` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL,
  `otype` int(11) NOT NULL,
  `ouid` int(11) NOT NULL,
  `model` int(11) NOT NULL,
  `name` varchar(64) NOT NULL,
  `url` varchar(128) NOT NULL DEFAULT 'brak',
  `entrycost` int(11) NOT NULL,
  `close` int(11) NOT NULL,
  `cars` int(11) NOT NULL,
  `enterx` float NOT NULL,
  `entery` float NOT NULL,
  `enterz` float NOT NULL,
  `entera` float NOT NULL,
  `exitx` float NOT NULL,
  `exity` float NOT NULL,
  `exitz` float NOT NULL,
  `exita` float NOT NULL,
  `entervw` int(11) NOT NULL,
  `enterint` int(11) NOT NULL,
  `exitvw` int(11) NOT NULL,
  `exitint` int(11) NOT NULL,
  `objectsmax` int(11) NOT NULL DEFAULT '200',
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `doors`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `duty_history`
--

CREATE TABLE IF NOT EXISTS `duty_history` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  `group` int(11) NOT NULL,
  `minutes` int(11) NOT NULL,
  `date` varchar(16) NOT NULL,
  `time` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `duty_history`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `groups_list`
--

CREATE TABLE IF NOT EXISTS `groups_list` (
  `group_uid` int(11) NOT NULL AUTO_INCREMENT,
  `group_name` varchar(64) NOT NULL,
  `group_tag` varchar(6) NOT NULL,
  `group_type` int(16) NOT NULL,
  `group_money` int(16) NOT NULL,
  `group_color` int(16) NOT NULL DEFAULT '808280',
  PRIMARY KEY (`group_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `groups_list`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `groups_workers`
--

CREATE TABLE IF NOT EXISTS `groups_workers` (
  `group_uid` int(12) NOT NULL,
  `char_uid` int(12) NOT NULL,
  `char_rank` varchar(32) NOT NULL,
  `char_payday` int(12) NOT NULL,
  `char_skin` int(12) NOT NULL,
  `char_perms` varchar(32) NOT NULL,
  `char_dutytime` int(12) NOT NULL,
  `group_name` varchar(64) NOT NULL,
  `last_pd` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin2;

--
-- Zrzut danych tabeli `groups_workers`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `houseinvites`
--

CREATE TABLE IF NOT EXISTS `houseinvites` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dooruid` int(11) NOT NULL DEFAULT '0',
  `charuid` int(11) NOT NULL DEFAULT '0',
  `charname` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `houseinvites`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `icons`
--

CREATE TABLE IF NOT EXISTS `icons` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `markertype` int(11) NOT NULL,
  `style` int(11) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `icons`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `labels`
--

CREATE TABLE IF NOT EXISTS `labels` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` varchar(256) NOT NULL,
  `vw` int(11) NOT NULL,
  `posx` float NOT NULL,
  `posy` float NOT NULL,
  `posz` float NOT NULL,
  `ownertype` int(11) NOT NULL,
  `owneruid` int(11) NOT NULL,
  `color` int(11) NOT NULL,
  `distance` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `labels`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `log_admincmd`
--

CREATE TABLE IF NOT EXISTS `log_admincmd` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` varchar(384) NOT NULL,
  `typer` int(11) NOT NULL,
  `date` varchar(16) NOT NULL,
  `time` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `log_admincmd`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `log_logowania`
--

CREATE TABLE IF NOT EXISTS `log_logowania` (
  `log_id` int(11) NOT NULL AUTO_INCREMENT,
  `log_gracz` int(11) NOT NULL,
  `log_success` int(5) NOT NULL,
  `log_data` varchar(16) NOT NULL,
  `log_czas` varchar(16) NOT NULL,
  `log_uwagi` varchar(128) NOT NULL,
  `log_ip` varchar(32) NOT NULL,
  PRIMARY KEY (`log_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `log_logowania`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `log_money`
--

CREATE TABLE IF NOT EXISTS `log_money` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `player` int(11) NOT NULL,
  `before` int(11) NOT NULL,
  `after` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `from` int(11) NOT NULL COMMENT '3 =gracz 2 = konto swoje 1 = konto grupy',
  `date` varchar(16) NOT NULL,
  `time` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `log_money`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `log_privates`
--

CREATE TABLE IF NOT EXISTS `log_privates` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` varchar(256) NOT NULL,
  `typer` int(11) NOT NULL,
  `receiver` int(11) NOT NULL,
  `date` varchar(16) NOT NULL,
  `time` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `log_privates`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `log_texting`
--

CREATE TABLE IF NOT EXISTS `log_texting` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` varchar(384) NOT NULL,
  `typer` int(11) NOT NULL,
  `date` varchar(16) NOT NULL,
  `time` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `log_texting`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `log_vehdmg`
--

CREATE TABLE IF NOT EXISTS `log_vehdmg` (
  `log_uid` int(11) NOT NULL AUTO_INCREMENT,
  `log_vuid` int(11) NOT NULL,
  `log_charuid` int(11) NOT NULL,
  `log_time` varchar(16) NOT NULL,
  `log_date` varchar(16) NOT NULL,
  `log_state` int(11) NOT NULL,
  `log_who` int(11) NOT NULL,
  `log_oldhp` float NOT NULL,
  `log_newhp` float NOT NULL,
  PRIMARY KEY (`log_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `log_vehdmg`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `log_vehspawn`
--

CREATE TABLE IF NOT EXISTS `log_vehspawn` (
  `log_uid` int(11) NOT NULL AUTO_INCREMENT,
  `log_vuid` int(11) NOT NULL,
  `log_charuid` int(11) NOT NULL,
  `log_time` varchar(16) NOT NULL,
  `log_date` varchar(16) NOT NULL,
  `log_state` int(5) NOT NULL,
  `log_who` int(5) NOT NULL,
  PRIMARY KEY (`log_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `log_vehspawn`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `lspd_arrests`
--

CREATE TABLE IF NOT EXISTS `lspd_arrests` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `typer` int(11) NOT NULL,
  `gainer` int(11) NOT NULL,
  `minutes` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `lspd_arrests`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `lspd_kartoteka`
--

CREATE TABLE IF NOT EXISTS `lspd_kartoteka` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `player` int(11) NOT NULL,
  `reason` varchar(64) NOT NULL,
  `cost` int(11) NOT NULL,
  `points` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `lspd_kartoteka`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `motels`
--

CREATE TABLE IF NOT EXISTS `motels` (
  `owner` int(11) NOT NULL AUTO_INCREMENT,
  `cost` int(11) NOT NULL DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '0',
  `dooruid` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`owner`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `motels`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `mysa_members`
--

CREATE TABLE IF NOT EXISTS `mysa_members` (
  `member_id` mediumint(8) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL DEFAULT '',
  `gmaccess` int(5) NOT NULL DEFAULT '0',
  `premium` int(11) NOT NULL DEFAULT '0',
  `gamescore` int(16) NOT NULL DEFAULT '0',
  `member_group_id` smallint(3) NOT NULL DEFAULT '0',
  `email` varchar(150) NOT NULL DEFAULT '',
  `joined` int(10) NOT NULL DEFAULT '0',
  `ip_address` varchar(46) NOT NULL DEFAULT '',
  `posts` mediumint(7) DEFAULT '0',
  `title` varchar(64) DEFAULT NULL,
  `allow_admin_mails` tinyint(1) DEFAULT NULL,
  `time_offset` varchar(10) DEFAULT NULL,
  `skin` smallint(5) DEFAULT NULL,
  `warn_level` int(10) DEFAULT NULL,
  `warn_lastwarn` int(10) NOT NULL DEFAULT '0',
  `language` mediumint(4) DEFAULT NULL,
  `last_post` int(10) DEFAULT NULL,
  `restrict_post` varchar(100) NOT NULL DEFAULT '0',
  `view_sigs` tinyint(1) DEFAULT '1',
  `view_img` tinyint(1) DEFAULT '1',
  `bday_day` int(2) DEFAULT NULL,
  `bday_month` int(2) DEFAULT NULL,
  `bday_year` int(4) DEFAULT NULL,
  `msg_count_new` int(2) NOT NULL DEFAULT '0',
  `msg_count_total` int(3) NOT NULL DEFAULT '0',
  `msg_count_reset` int(1) NOT NULL DEFAULT '0',
  `msg_show_notification` int(1) NOT NULL DEFAULT '0',
  `misc` varchar(128) DEFAULT NULL,
  `last_visit` int(10) DEFAULT '0',
  `last_activity` int(10) DEFAULT '0',
  `dst_in_use` tinyint(1) DEFAULT '0',
  `coppa_user` tinyint(1) DEFAULT '0',
  `mod_posts` varchar(100) NOT NULL DEFAULT '0',
  `auto_track` varchar(50) DEFAULT '0',
  `temp_ban` varchar(100) DEFAULT '0',
  `login_anonymous` char(3) NOT NULL DEFAULT '0&0',
  `ignored_users` text,
  `mgroup_others` varchar(255) NOT NULL DEFAULT '',
  `org_perm_id` varchar(255) NOT NULL DEFAULT '',
  `member_login_key` varchar(32) NOT NULL DEFAULT '',
  `member_login_key_expire` int(10) NOT NULL DEFAULT '0',
  `has_blog` text,
  `blogs_recache` tinyint(1) DEFAULT NULL,
  `has_gallery` tinyint(1) NOT NULL DEFAULT '0',
  `members_auto_dst` tinyint(1) NOT NULL DEFAULT '1',
  `members_display_name` varchar(255) NOT NULL DEFAULT '',
  `members_seo_name` varchar(255) NOT NULL DEFAULT '',
  `members_created_remote` tinyint(1) NOT NULL DEFAULT '0',
  `members_cache` mediumtext,
  `members_disable_pm` int(1) NOT NULL DEFAULT '0',
  `members_l_display_name` varchar(255) NOT NULL DEFAULT '',
  `members_l_username` varchar(255) NOT NULL DEFAULT '',
  `failed_logins` text,
  `failed_login_count` smallint(3) NOT NULL DEFAULT '0',
  `members_profile_views` int(10) unsigned NOT NULL DEFAULT '0',
  `members_pass_hash` varchar(32) NOT NULL DEFAULT '',
  `members_pass_salt` varchar(5) NOT NULL DEFAULT '',
  `member_banned` tinyint(1) NOT NULL DEFAULT '0',
  `member_uploader` varchar(32) NOT NULL DEFAULT 'default',
  `members_bitoptions` int(10) unsigned NOT NULL DEFAULT '0',
  `fb_uid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `fb_emailhash` varchar(60) NOT NULL DEFAULT '',
  `fb_lastsync` int(10) NOT NULL DEFAULT '0',
  `members_day_posts` varchar(32) NOT NULL DEFAULT '0,0',
  `live_id` varchar(32) DEFAULT NULL,
  `twitter_id` varchar(255) NOT NULL DEFAULT '',
  `twitter_token` varchar(255) NOT NULL DEFAULT '',
  `twitter_secret` varchar(255) NOT NULL DEFAULT '',
  `notification_cnt` mediumint(9) NOT NULL DEFAULT '0',
  `tc_lastsync` int(10) NOT NULL DEFAULT '0',
  `fb_session` varchar(200) NOT NULL DEFAULT '',
  `fb_token` text,
  `ips_mobile_token` varchar(64) DEFAULT NULL,
  `unacknowledged_warnings` tinyint(1) DEFAULT NULL,
  `ipsconnect_id` int(10) NOT NULL DEFAULT '0',
  `ipsconnect_revalidate_url` text,
  PRIMARY KEY (`member_id`),
  KEY `members_l_display_name` (`members_l_display_name`),
  KEY `members_l_username` (`members_l_username`),
  KEY `mgroup` (`member_group_id`,`member_id`),
  KEY `member_groups` (`member_group_id`,`mgroup_others`),
  KEY `bday_day` (`bday_day`),
  KEY `bday_month` (`bday_month`),
  KEY `member_banned` (`member_banned`),
  KEY `members_bitoptions` (`members_bitoptions`),
  KEY `ip_address` (`ip_address`),
  KEY `failed_login_count` (`failed_login_count`),
  KEY `joined` (`joined`),
  KEY `fb_uid` (`fb_uid`),
  KEY `twitter_id` (`twitter_id`),
  KEY `email` (`email`),
  KEY `blogs_recache` (`blogs_recache`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `mysa_members`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `penalties`
--

CREATE TABLE IF NOT EXISTS `penalties` (
  `penalty_id` int(11) NOT NULL AUTO_INCREMENT,
  `penalty_date` varchar(16) NOT NULL,
  `penalty_time` varchar(16) NOT NULL,
  `penalty_gainer` int(11) NOT NULL,
  `penalty_giver` int(11) NOT NULL,
  `penalty_reason` varchar(64) NOT NULL,
  `penalty_type` varchar(16) NOT NULL,
  `penalty_expired` int(1) NOT NULL,
  PRIMARY KEY (`penalty_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `penalties`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `petrolstation`
--

CREATE TABLE IF NOT EXISTS `petrolstation` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `vw` int(11) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `petrolstation`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `purchases`
--

CREATE TABLE IF NOT EXISTS `purchases` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `doorid` int(11) NOT NULL,
  `val1` int(11) NOT NULL,
  `val2` int(11) NOT NULL,
  `name` varchar(32) NOT NULL,
  `type` int(11) NOT NULL,
  `howmuch` int(11) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `purchases`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `srv_settings`
--

CREATE TABLE IF NOT EXISTS `srv_settings` (
  `id` int(5) NOT NULL AUTO_INCREMENT,
  `type` int(5) NOT NULL DEFAULT '0',
  `val` int(16) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `srv_settings`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `tel_contacts`
--

CREATE TABLE IF NOT EXISTS `tel_contacts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `numer` int(11) NOT NULL,
  `c_number` int(11) NOT NULL,
  `c_nazwa` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `tel_contacts`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `uprawnienia`
--

CREATE TABLE IF NOT EXISTS `uprawnienia` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `gid` int(11) NOT NULL DEFAULT '0',
  `pojazdy` int(11) NOT NULL DEFAULT '0',
  `grupy` int(11) NOT NULL DEFAULT '0',
  `przedmioty` int(11) NOT NULL DEFAULT '0',
  `drzwi` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `uprawnienia`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `vehicles_list`
--

CREATE TABLE IF NOT EXISTS `vehicles_list` (
  `veh_uid` int(11) NOT NULL AUTO_INCREMENT,
  `veh_sampid` int(11) NOT NULL,
  `veh_hp` float NOT NULL,
  `veh_owner` int(11) NOT NULL,
  `veh_ownertype` int(5) NOT NULL,
  `veh_spawned` int(5) NOT NULL,
  `veh_name` varchar(32) NOT NULL,
  `veh_model` int(11) NOT NULL,
  `veh_x` float NOT NULL,
  `veh_y` float NOT NULL,
  `veh_z` float NOT NULL,
  `veh_angle` float NOT NULL,
  `veh_col1` int(11) NOT NULL,
  `veh_col2` int(11) NOT NULL,
  `veh_paintjob` int(5) NOT NULL DEFAULT '3',
  `veh_plates` varchar(24) NOT NULL,
  `veh_fuel` float NOT NULL,
  `dm_panels` int(11) NOT NULL,
  `dm_doors` int(11) NOT NULL,
  `dm_lights` int(11) NOT NULL,
  `dm_tires` int(11) NOT NULL,
  `veh_world` int(11) NOT NULL,
  `veh_przebieg` int(32) NOT NULL,
  `veh_started` int(5) NOT NULL,
  `veh_windows` int(1) NOT NULL DEFAULT '0',
  `veh_engine` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`veh_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `vehicles_list`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `veh_blocks`
--

CREATE TABLE IF NOT EXISTS `veh_blocks` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `cost` int(11) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `veh_blocks`
--


-- --------------------------------------------------------

--
-- Struktura tabeli dla  `vlive_anim`
--

CREATE TABLE IF NOT EXISTS `vlive_anim` (
  `anim_uid` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `anim_command` varchar(12) COLLATE utf8_unicode_ci NOT NULL,
  `anim_lib` varchar(16) COLLATE utf8_unicode_ci NOT NULL,
  `anim_name` varchar(24) COLLATE utf8_unicode_ci NOT NULL,
  `anim_speed` float NOT NULL,
  `anim_opt1` tinyint(1) NOT NULL,
  `anim_opt2` tinyint(1) NOT NULL,
  `anim_opt3` tinyint(1) NOT NULL,
  `anim_opt4` tinyint(1) NOT NULL,
  `anim_opt5` tinyint(1) NOT NULL,
  `anim_action` tinyint(1) NOT NULL,
  PRIMARY KEY (`anim_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

--
-- Zrzut danych tabeli `vlive_anim`
--

