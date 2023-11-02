#include "locale.h"


namespace Game {
    
std::array<std::string, Engine::BONUS_MAX> Locale::BonusName = 
{ " Хар", " Лов", " Хар", " Здр", " Сил", " Вын", " Брн", " Удр", " Вер" };

std::array<std::string, 20> Locale::NewGameMenu = 
{ "Герой Северного клана, сильнейшего во времена Титанов. Минули столетия, и слава древних Богов стерлась из памяти Драгомира. Все это время он собирал вокруг себя лучших воинов земли, готовясь к решающей битве за главенство в клане. Опасаясь вновь стать смертным, Драгомир прибыл в Лесную страну на поиски амулета Дракона."
, "Старый друг Всеслава и верный товарищ по оружию. Оставив военные походы, Михаил занялся изучением наук. Но, поняв, что судьба мира Титанов зависит от исхода решающей битвы за амулет Дракона, прибыл в Лесную страну с отрядом ветеранов, охраняющих византийский торговый пост."
, "Последний из клана Охотников. Со времен Титанов Волк славился искусством стрельбы из лука. Живет в Лесной стране и мечтает возродить свой клан. Слава о нем далеко разнеслась по округе, и многие могли бы пойти за ним. Теперь Волк полон решимости вступить в бой за амулет Дракона, даже если воевать придется с берсерками Драгомира."
, "Не так ловок, как Охотник и не столь силен, как Воин, но очень вынослив. Купец способен выдержать сильнейшие удары вражеского меча. Не имеет себе равных по тяжести поклажи, которую может носить с собой."
, "Охотник Лесной страны прекрасно владеет луком. В ближнем бою он не может нанести такой же сильный удар, как воин, но его удары точнее, и он легко уворачивается от вражеского меча."
, "Главное достоинство вождя - сила его убеждающего слова. Вождь может собрать самую большую дружину, и рядом с ним лучшие бойцы будут биться до последней капли крови."
, "Меч воина способен рубить камни, а во время боя создается впечатление, что сражается сам Титан. Воин может погибнуть, но прежде чем это случится, тысячи неприятелей окажутся поверженными."
, "Харизма - врожденное свойство вождя, способность повести за собой, увлечь. От величины этого параметра зависит количество воинов, которых ты сможешь собрать под своим началом."
, "От твоей физической силы зависит, с какой мощью ты сможешь опустить меч или топор на голову врага. Удар сильного разрубает вражеский панцирь, как соломенную корзину. Самыми сильными воинами Лесной страны стали викинги, приплывшие сюда с Драгомиром."
, "Сможешь ли ты увернуться от острия вражеского меча или летящей в тебя стрелы, зависит только от твоей ловкости. Точно выстрелить из лука и поразить врага способен лишь ловкий боец. Местные охотники по праву считаются самыми лучшими стрелками Лесной страны."
, "Чем выносливее герой, тем больше ударов врага он выдержит. Может нести больше поклажи, носить лучшие латы и драться самым тяжелым оружием, проламывающем стальные шлемы недругов, как яичную скорлупу. Наиболее выносливыми и привычными к тяжелым долгим походам стали воины, охраняющие византийский торговый пост."
, "В странствиях по Лесной стране тебе придется носить тяжелые латы, оружие и щит. В заплечном мешке много места займут самые разные вещи, необходимые воину в дороге. Не так уж просто бродить по лесами рубить врагов и чудовищ с тяжелой поклажей. Все будет зависеть от твоей выносливости."
, "Нелегко собрать большую, хорошо обученную дружину, без которой не добыть амулет Дракона, не пройти через леса и пещеры, населенные страшными чудовищами. Только за настоящим вождем пойдут люди в бой, доверяя ему свои жизни."
, "Настоящий боец точит и чинит свое оружие сам. Правильный уход за мечом и латами намного продлит их срок службы."
, "Хороший лучник должен уметь подтянуть ослабшую тетиву, поправить оперение стрелы, заточить наконечник. Негоже герою за каждой мелочью бегать к мастеру."
, "Мастерство владения мечом или топором. Хорошо натренированный боец стоит трех необученных. Совершенствуй свое мастерство на лесных гадах и готовься к сражению за амулет Дракона."
, "Лук в руках мастера - грозное оружие. Стань лучшим стрелком в Лесной стране. Стреляй чаще и не жалей стрел. Помни также, что горящими стрелами можно дотла спалить лагерь недругов."
, "Распознать скрытые возможности Поющих вещей, оставшихся от эпохи Титанов, далеко не так просто, как кажется. Только герой, повидавший в жизни много Поющих вещей, способен определить, какую пользу или вред принесут они своему хозяину."
, "Уметь торговаться не постыдно даже самым отважным воинам. Купить дешево дорогой меч, собрать большую дань с деревни - важные слагаемые успеха в нелегкой борьбе за обладание амулетом Дракона."
, "Странствующий по лесам и пещерам герой должен уметь сам лечить свои раны. Труден и долог может быть путь к ближайшему знахарю. А умение приготовить лечебную или отравляющую смесь самостоятельно поможет сэкономить и время и деньги."};

std::array<std::string, 70> Locale::WeapArmorNames =
{"Гладий" ,"Спата" ,"Меч" ,"Топор" ,"Бердициум" ,"Палица" ,"Короткий лук" ,"Пехотный лук" ,"Булатный лук" ,"Кинжал"
,"Скармасакс" ,"Длинный меч" ,"Палица" ,"Топор" ,"Секира" ,"Охотничий лук" ,"Длинный лук" ,"Составной лук" ,"Нож" ,"Меч"
,"Меч" ,"Дубина" ,"Топор" ,"Палица" ,"Короткий лук" ,"Составной лук" ,"Клееный лук" ,"Богатырский меч" ,"Богатырский топор" ,"Богатырский лук"
," кремниевые стрелы" ," медные стрелы" ," железные стрелы" ," серебряные стрелы" ,"Доспех кожаный" ,"Доспех кожано-металлический" ,"Кольчуга" ,"Щит малый" ,"Щит строевой" ,"Элептус"
,"Скутум" ,"Шлем кожаный" ,"Каска" ,"Шлем гребневый" ,"Шлем катафрактный" ,"Доспех кожаный" ,"Доспех кожано-металлический" ,"Кольчуга" ,"Деревянный щит" ,"Деревянный щит"
,"Щит с умбоном" ,"Щит длинный" ,"Шлем кожаный" ,"Шлем клепаный" ,"Шлем яйцевидный" ,"Шлем датский" ,"Доспех кожаный" ,"Доспех кожано-металлический" ,"Кольчуга" ,"Щит кожаный"
,"Щит круглый" ,"Щит овальный" ,"Щит миндалевидный" ,"Железный шлем" ,"Шлем клепаный" ,"Шлем кольчужный" ,"Шишак" ,"Богатырские доспехи" ,"Богатырский щит" ,"Богатырский шлем" };
        

}