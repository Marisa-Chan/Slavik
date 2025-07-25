#include "game_locale.h"


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
{     "Гладий" ,"Спата" ,"Меч" ,"Топор" ,"Бердициум" ,"Палица" ,"Короткий лук" ,"Пехотный лук" ,"Булатный лук" 
/*9*/ ,"Кинжал" ,"Скармасакс" ,"Длинный меч" ,"Палица" ,"Топор" ,"Секира" ,"Охотничий лук" ,"Длинный лук" ,"Составной лук"
/*18*/,"Нож" ,"Меч" ,"Меч" ,"Дубина" ,"Топор" ,"Палица" ,"Короткий лук" ,"Составной лук" ,"Клееный лук" 
/*27*/,"Богатырский меч" ,"Богатырский топор" ,"Богатырский лук"
/*30*/," кремниевые стрелы" ," медные стрелы" ," железные стрелы" ," серебряные стрелы" 
/*34*/,"Доспех кожаный" ,"Доспех кожано-металлический" ,"Кольчуга" ,"Щит малый" ,"Щит строевой" ,"Элептус"
/*40*/,"Скутум" ,"Шлем кожаный" ,"Каска" ,"Шлем гребневый" ,"Шлем катафрактный" 
/*45*/,"Доспех кожаный" ,"Доспех кожано-металлический" ,"Кольчуга" ,"Деревянный щит" ,"Деревянный щит" ,"Щит с умбоном"
/*51*/,"Щит длинный" ,"Шлем кожаный" ,"Шлем клепаный" ,"Шлем яйцевидный" ,"Шлем датский"
/*56*/,"Доспех кожаный" ,"Доспех кожано-металлический" ,"Кольчуга" ,"Щит кожаный" ,"Щит круглый" ,"Щит овальный"
/*62*/,"Щит миндалевидный" ,"Железный шлем" ,"Шлем клепаный" ,"Шлем кольчужный" ,"Шишак"
/*67*/,"Богатырские доспехи" ,"Богатырский щит" ,"Богатырский шлем" };
  
std::array<std::string, 9> Locale::AcessoriesInfoNames =
{"Ожерелье", "Ожерелье", "Ожерелье",
"Браслет", "Браслет", "Браслет",
"Кольцо", "Кольцо", "Кольцо"};


std::array<std::string, 14> Locale::AlchemiesInfoNames =
{"Ядовитое жало", "Земляной орех", "Белый корень",
"Пустая банка", "Непонятная смесь", 
"Лечебный бальзам", "Яд", "Масло",
"Противоядие", "Брага", "Зелье",
"Чистая слеза", "Элексир Мудрости", "Живая Вода"};

std::array<std::string, 54> Locale::MiscItemsInfoNames =
{"Потерянная карта", "Труды Гиппократа", "Чертежи древних пирамид",
"Свиток кузнеца", "Трактат о торговле", "Ягода Третий Глаз",
"Слова Воскрешения", "Дупликатор", "Соколиная лапка",
"Свиток ведуна", "Яблоко познания", "Сын Луны",
"Вино", "Великий магнит", "Магический шар волхва Бережков",
"Магический шар волхва Старого лагеря", 
"Магический шар волхва Византийского лагеря",
"Магический шар волхва Черного бора",
"Магический шар волхва Родников",
"Магический шар волхва Лесного лагеря",
"Шар магического прыжка", "Магический шар Подземного города",
"Рукоять древнего меча", "Лезвие древнего меча", "Лопата",
"Правая часть амулета Дракона", "Левая часть амулета Дракона",
"Нижняя часть амулета Дракона",
"Берестяная грамота", "Медная лампа", "Медная лампа с именем Короля скелетов",
"Факел", "Неразменный пятак", "Кукла", "Заячий хвост", "Свисток",
"Чаша воеводы", "Дудочка", "Святые мощи", "Детские игрушки",
"Амулет Дракона", "Левая и правая части амулета Дракона",
"Правая и нижняя части амулета Дракона",
"Левая и нижняя части амулета Дракона",
"Ключ Воды", "Перо Жар-птицы", "Клык белого волка", "Паутина странного паука",
"Медное зеркало колдуна", "Волшебная вода", "Ларец колдуна", "Камень",
"Ключ Луны", "Ключ Огня"};

std::array<std::string, 18> Locale::SpecialDamagesDescr =
{
/* 0*/", зажигательные", ", на оборотня", ", на муравьев", ", на ичетика",
/* 4*/", на пауков", ", на червяков", ", на морока", ", на омутника",
/* 8*/", на скелетов",  ", на навей", ", на мертвяка", ", на моховика",
/*12*/", на аспида", ", на лешего", ", на болотника", ", на цветка-людоеда",
/*16*/", на ядовитых пауков", ", на ядовитых червяков"
};

std::array<std::string, 30> Locale::MonsterNames =
{
/* 0*/"Гигантский Муравей", "Аспид", "Болотник", "Цветок-людоед", "Мертвяк",
/* 5*/"Ичетик", "Леший", "Моховик", "Морок", "Навья",
/*10*/"Омутник", "Скелет", "Паук", "Гигантский Червь", "Ядовитый паук",
/*15*/"Ядовитый червь", "Ребенок", "Женщина", "Волхв", "Дракон",
/*20*/"", "", "", "", "",
/*25*/"", "", "", "", ""
};

std::array<std::string, 171> Locale::PeopleNames =
{
/* 0*/"", "Белун", "Блуд", "Бодрич", "Боян", 
/* 5*/"Велигост", "Велимир", "Вилинец", "Владимир", "Волеслав",
/*10*/"Волк", "Вольга", "Всеслав", "Вячеслав", "Глеб",
/*15*/"Гостомысл", "Добрыня", "Зорькин", "Изяслав", "Исток",
/*20*/"Коруша", "Крок", "Ладомир", "Лучезар", "Лучеслав",
/*25*/"Мал", "Микула", "Миромир", "Мстислав", "Никита",
/*30*/"Ослябя", "Охлябий", "Перемысл", "Пересвет", "Позвизд",
/*35*/"Путята", "Радагаст", "Радигощ", "Радо", "Ратибор",
/*40*/"Ростислав", "Сварун", "Световик", "Светогор", "Светозар",
/*45*/"Святовит", "Святополк", "Святослав", "Орей", "Силянин",
/*50*/"Слав", "Славун", "Таргитай", "Томил", "Тур",
/*55*/"Хорив", "Щек", "Яромир", "Ярополк", "Ярослав",
/*60*/"Ярун", "Святослава", "Светослава", "Любница", "Людмила",
/*65*/"Малуша", "Ольга", "Маланья", "Ярославна", "Велиславна",
/*70*/"Лыбедь", "Власта", "Влада", "Олеся", "Марфа",
/*75*/"Адльстайн", "Акки", "Аринбьорн", "Асвид", "Аслак",
/*80*/"Атли", "Бриньольв", "Бьорн", "Вив", "Водурид",
/*85*/"Вулферт", "Гилли", "Грани", "Ивар", "Ингви",
/*90*/"Йон", "Кетиль", "Лейф", "Магнус", "Олав",
/*95*/"Освик", "Оттар", "Рагнар", "Свенвельд", "Сигвальд",
/*100*/"Сигмар", "Сигурд", "Снорри", "Стурла", "Сэмунд",
/*105*/"Торлейф", "Торнбьорн", "Торрар", "Торстейн", "Трюгви",
/*110*/"Тунд", "Тьяльви", "Ульв", "Хакон", "Хальдван",
/*115*/"Хальфгар", "Харальт", "Хатуволафр", "Хорнбор", "Хрорар",
/*120*/"Хрофт", "Эгиль", "Эймунд", "Эйнар", "Драгомир",
/*125*/"Страж ключа Огня", "Брюнхильд", "Гунхильд", "Страж ключа Воды", "Кримхильда",
/*130*/"Александр", "Страж Браслета", "Алексей", "Арефа", "Асбад",
/*135*/"Василий", "Велизарий", "Григорий", "Дмитрий", "Евстафий",
/*140*/"Ифгений", "Иоанн", "Страж ключа Луны", "Исак", "Кирилл",
/*145*/"Константин", "Вершитель", "Лев", "Мануил", "Мефодий",
/*150*/"Мирон", "Михаил", "Мунд", "Нарсез", "Нестор",
/*155*/"Прокоп", "Никифор", "Петр", "Федор", "Феофан",
/*160*/"Феофил", "Фока", "Фотий", "Эпофродит", "Максим",
/*165*/"Александра", "Анастасия", "Ифигения", "Мария", "Прасковья",
/*170*/"Федора"
};

std::array<std::string, 21> Locale::PeopleNicks =
{
/* 0*/"", "Косой", "Кривой", "Глухой", "Слепой",
/* 5*/"Сирота", "Кузнец", "Медведь", "Ворон", "Деревянный зуб",
/*10*/"Кожаные штаны", "Кормчий", "Кровавый топор", "Острый клык", "Рыжий",
/*15*/"Счастливый", "Собака", "Тюлень", "Тяжелорукий", "Волчий клык",
/*20*/"Рваный"
};

std::array<std::string, 5> Locale::CharacterSpeciality =
{"Храбрец", "Купец", "Охотник", "Вождь", "Воин"};

std::array<std::string, 42> Locale::BuildingInfoNames =
{
/* 0*/"Дом старосты", "Дом старосты", "Палатка старосты", "Изба знахаря", "Палатка знахаря",
/* 5*/"Лавка купца",  "Торговая палатка", "Кузница", "Кузница", "Кузница",
/*10*/"Казарма", "Казарма", "Казарма", "Казарма", "Мост",
/*15*/"Мост", "Колодец", "Колодец", "Колодец", "Частокол",
/*20*/"Частокол", "Частокол", "Частокол", "Частокол", "Частокол",
/*25*/"Частокол", "Частокол", "Славянская изба", "Славянская изба", "Славянская изба",
/*30*/"Славянская изба", "Византийская палатка", "Дом викингов", "Дом викингов", "Дом викингов",
/*35*/"Флагшток", "Загон для животных", "Загон для животных", "Сторожевая башня", "Сторожевая башня",
/*40*/"Сторожевая башня", "Сторожевая башня"
};

std::array<std::string, Locale::GMSG_MAX> Locale::GameMessages = 
{
/*GMSG_VLG_GIVEUP*/"Довольно крови, князь. Деревня сдается на твою милость!",
/*GMSG_INV_OVERLOAD*/"Превышен вес",
/*GMSG_INV_FULL*/"Мешок у тебя не резиновый!",
/*GMSM_SMTH_LOOT*/"Что-то лежит на земле",
/*GMSM_ALL_TO_ME*/"Все ко мне (F)",
/*GMSM_ATTACK*/"Атаковать (A)",
/*GMSM_THINK_INFO*/"Информация к размышлению (Q)",
/*GMSM_CHAR_PANEL*/"Панель персонажа (I)",
/*GMSM_CHANGE_WPN*/"Сменить оружие (1, 2, 3)",
/*GMSM_DRAW_WEAPON*/"Достать/Убрать оружие (Пробел)",
/*GMSM_MAP*/"Карта (M)",
/*GMSM_UNUSABLE*/"Предмет не доступен для использования",
/*GMSM_MEDICTEACH*/"Ты открыл для себя секреты древних целителей",
/*GMSM_PLOTNTEACH*/"Ты познал секреты строительного искусства",
/*GMSM_BLKSMTHTEACH*/"Ты познал секреты кузнечного дела",
/*GMSM_TRADETEACH*/"Ты узнал много нового об искусстве торговли",
/*GMSM_IDENTIFYTEACH*/"Ты узнал много интересного о неведомых вещах",
/*GMSM_READBIRCHBARK*/"Ты прочитал Берестяную Грамоту",
/*GMSM_NOMONEY*/"Слишком мало даешь!"
};

std::array<std::string, Locale::CHARHINT_MAX> Locale::CharHints =
{
/*GMSM_HP_LVL*/", Уров:%d, Здор:%d",
/*CHAR_SQUAD_UNIT*/", воин дружины",
/*CHAR_POISONED*/", отравлен",
/*CHAR_ENEMY*/", враг",
/*CHAR_KNOW_MEDICINE*/", знаком со знахарским делом",
/*CHAR_KNOW_BLACKSM*/", знает кузнечное дело",
/*CHAR_KNOW_TRADING*/", опытен в торговле",
/*CHAR_KNOW_CONSTRUCT*/", искусный строитель",
/*CHAR_KNOW_IDENTIFY*/", разбирается в диковинных вещах"
};

std::array<std::string, 5> Locale::Jobs =
{
", староста", ", знахарь", ", купец", ", кузнец", ", воевода",
};

std::array<std::string, 56> Locale::MapNames =
{
/* 0*/"Родники", "Укромное место", "Волхв у Родников", "Старый лагерь", "Волхв у Старого лагеря",
/* 5*/"Мыс", "Византийский Лагерь", "Полуостров", "Болото у Византийского Лагеря", "Волхв у Византийского Лагеря",
/*10*/"Ловье", "Волхв у Ловье", "Поляны у Ловье", "Верхний лагерь", "Волхв у Верхнего лагеря",
/*15*/"Лесовье", "Волхв у Лесовья", "Пепелище у Лесовья", "Черный Бор", "Волхв у Черного Бора",
/*20*/"Лагерь Волка", "Поречье", "Волхв у Поречья", "Куницын Бор", "Волхв у Куницыного Бора",
/*25*/"Островок на болоте", "Бережки", "Волхв у Бережков", "Камни", "Волхв у Камней",
/*30*/"Пещера у Камней", "Волхв у Пропасти", "Борье", "Рудник у Борья", "Средний лагерь",
/*35*/"Волхв у среднего лагеря", "Нижний лагерь", "Волхв у Нижнего лагеря", "Лесной лагерь", "Волхв у Лесного лагеря",
/*40*/"Гнездо ", "Пещера Воды", "Пещера Зверя", "Пещера Луны", "Пещера Огня",
/*45*/"Гнездо Дракона", "Старая шахта", "Гнездо Дракона", "Гнездо Дракона", "Гнездо Дракона",
/*50*/"Пещера №10", "Пещера №11", "Пещера №12", "Пещера №13", "Пещера №14",
/*55*/"Остров"
};


std::array<std::string, Locale::ITMDES_MAX> Locale::ItemDescr =
{
/*ITMDES_DMG_FORCE*/", убойная сила ",
/*ITMDES_ARMR*/", броня: ",
/*ITMDES_WEAR*/", износ ",
/*ITMDES_WEIGHT*/", вес ",
/*ITMDES_POISON*/", отравление: ",
/*ITMDES_NEEDS*/", требует: ",
/*ITMDES_UNK_WPN*/"Неведомое оружие",
/*ITMDES_UNK_BOW*/"Неведомый лук",
/*ITMDES_UNK_ITEM*/"Неведомый предмет",
/*ITMDES_CONCENTRATION*/", концентрация ",
/*ITMDES_INS_CONC*/", недостаточная концентрация",
/*ITMDES_QUIVER*/"Колчан стрел: ",
/*ITMDES_UNK_ARROWS*/" неведомые стрелы"
};

}