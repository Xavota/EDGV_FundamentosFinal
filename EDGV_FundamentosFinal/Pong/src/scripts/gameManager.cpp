#include "scripts/gameManager.h"

#include "pongFramework.h"

#include <platform/iofile.h>

#include <tools/textureManager.h>
#include <tools/fontManager.h>

#include <scene/scene.h>
#include <scene/actor.h>
#include <scene/components/transform.h>

#include <scene/components/canvas.h>
#include <scene/components/render.h>
#include <scene/components/shapeRect.h>
#include <scene/components/uiText.h>
#include <scene/components/button.h>

#include "scripts/statesPattern/stateMachine.h"

#include "scripts/gameMap.h"
#include "scripts/mapBuilder.h"
#include "scripts/mapEntities/moving/player.h"
#include "scripts/mapEntities/moving/phantoms/blinky.h"
#include "scripts/mapEntities/moving/phantoms/pinky.h"
#include "scripts/mapEntities/moving/phantoms/inky.h"
#include "scripts/mapEntities/moving/phantoms/clyde.h"

void GameManager::start()
{
  auto& framework = PongFramework::instance();
  auto& fontMan = gl::FontManager::instance();


  SPtr<Actor> stateMachine = getScene().lock()->addActor("stateMachine").lock();
  m_pStateMachine = stateMachine->addComponent<StateMachine>().lock();


  SPtr<Actor> gameMap = getScene().lock()->addActor("gameMap").lock();
  m_pGameMap = gameMap->addComponent<GameMap>().lock();
  SPtr<Transform> gameMapTrans = gameMap->getTransform().lock();
  gameMapTrans->setLocalPosition({0.5f, 4.5f});

  SPtr<Actor> mapBuilder = getScene().lock()->addActor("mapBuilder").lock();
  SPtr<MapBuilder> mapBuilderComp = mapBuilder->addComponent<MapBuilder>().lock();
  mapBuilderComp->init(m_sMapFile, m_pGameMap);

  m_pGameMap->init([this] () {
    std::cout << "Last Coin" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kGotLastCoin);
  });

  SPtr<Actor> player = getScene().lock()->addActor("player").lock();
  m_pPlayer = player->addComponent<Player>().lock();
  player->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pPlayer->init(m_pGameMap, 6.0f, [this](){ this->eatablePhantoms(); },
                  [this](){ playerDied(); },
                  [this](){ m_pStateMachine->externalInput(eEXTERNAL_INPUT::kPlayerDied); });
  
  SPtr<Actor> blinky = getScene().lock()->addActor("blinky").lock();
  m_pBlinky = blinky->addComponent<Blinky>().lock();
  blinky->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pBlinky->init(m_pGameMap, 5.5f, 0);
  
  SPtr<Actor> pinky = getScene().lock()->addActor("pinky").lock();
  m_pPinky = pinky->addComponent<Pinky>().lock();
  pinky->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pPinky->init(m_pGameMap, 5.5f, 0);
  
  SPtr<Actor> inky = getScene().lock()->addActor("inky").lock();
  m_pInky = inky->addComponent<Inky>().lock();
  inky->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pInky->init(m_pGameMap, 5.5f, 0);
  
  SPtr<Actor> clyde = getScene().lock()->addActor("clyde").lock();
  m_pClyde = clyde->addComponent<Clyde>().lock();
  clyde->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pClyde->init(m_pGameMap, 5.5f, 0);

  gameMap->setActive(false);


  SPtr<Actor> canvas = getScene().lock()->addActor("canvas").lock();
  canvas->addComponent<Canvas>();
  WPtr<Transform> canvasT = canvas->getTransform();


  makeMainMenu(canvasT);
  makePauseMenu(canvasT);
  makeGameMenu(canvasT);
  makeGameOverMenu(canvasT);
  makeGameWonMenu(canvasT);


  //
  // INIT GAME
  //

  m_pStateMachine->init(shared_from_this());
}

void GameManager::newGame()
{
  resetMovingObj();
  m_pPlayer->m_iLives = m_pPlayer->m_iOgLives;
  m_pGameMap->restart();
  prepareGame();
}
void GameManager::prepareGame()
{
  m_pGameMap->getActor().lock()->setActive(true);
}
void GameManager::startPlaying()
{
  m_pPlayer->setCanMove(true);
  m_pBlinky->setCanMove(true);
  m_pPinky->setCanMove(true);
  m_pInky->setCanMove(true);
  m_pClyde->setCanMove(true);
}
void GameManager::pause()
{
  m_pPlayer->setPaused(true);
  m_pBlinky->setPaused(true);
  m_pPinky-> setPaused(true);
  m_pInky->  setPaused(true);
  m_pClyde-> setPaused(true);
}
void GameManager::resume()
{
  m_pPlayer->setPaused(false);
  m_pBlinky->setPaused(false);
  m_pPinky-> setPaused(false);
  m_pInky->  setPaused(false);
  m_pClyde-> setPaused(false);
}

void GameManager::resetMovingObj()
{
  m_pPlayer->reset();
  m_pBlinky->reset();
  m_pPinky-> reset();
  m_pInky->  reset();
  m_pClyde-> reset();

  m_pBlinky->getActor().lock()->setActive(true);
  m_pPinky->getActor().lock()-> setActive(true);
  m_pInky->getActor().lock()->  setActive(true);
  m_pClyde->getActor().lock()-> setActive(true);

  m_pPlayer->setCanMove(false);
  m_pBlinky->setCanMove(false);
  m_pPinky->setCanMove (false);
  m_pInky->setCanMove  (false);
  m_pClyde->setCanMove (false);

  m_pPlayer->setPaused(false);
  m_pBlinky->setPaused(false);
  m_pPinky-> setPaused(false);
  m_pInky->  setPaused(false);
  m_pClyde-> setPaused(false);
}

bool GameManager::isGameOver()
{
  //std::cout << "isGameOver? m_pPlayer->m_iLives: " << m_pPlayer->m_iLives << std::endl;
  return m_pPlayer->m_iLives == 0;
}

void GameManager::loadGame()
{
  
}
void GameManager::saveGame()
{
  std::cout << "Save Game" << std::endl;

  File saveFile(L"../saves/saveFile.pac",
          eOPEN_TYPE::kBinary | eOPEN_TYPE::kWriteOnly | eOPEN_TYPE::kTruncate);

  // TODO: EVENTUALMENTE SE ACEPTARÁN MÁS ARCHIVOS DE MAPAS Y ESTO VA A SER
  //       NECESARIO
  /*U32 mapNameSize = static_cast<U32>(m_sMapFile.size());
  saveFile.writeBytes(reinterpret_cast<Byte*>(&mapNameSize),
                      sizeof(U32));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_sMapFile),
                      m_sMapFile.size() * sizeof(wchar_t));*/

  m_pGameMap->saveToFile(saveFile);
  m_pPlayer->saveToFile(saveFile);
  m_pBlinky->saveToFile(saveFile);
  m_pPinky->saveToFile(saveFile);
  m_pInky->saveToFile(saveFile);
  m_pClyde->saveToFile(saveFile);

  saveFile.close();
}


void GameManager::eatablePhantoms()
{
  m_pBlinky->makeEatable(8.0f);
  m_pPinky->makeEatable(8.0f);
  m_pInky->makeEatable(8.0f);
  m_pClyde->makeEatable(8.0f);
}

void GameManager::playerDied()
{
  m_pBlinky->getActor().lock()->setActive(false);
  m_pBlinky->setCanMove(false);
  m_pPinky->getActor().lock()->setActive(false);
  m_pPinky->setCanMove(false);
  m_pInky->getActor().lock()->setActive(false);
  m_pInky->setCanMove(false);
  m_pClyde->getActor().lock()->setActive(false);
  m_pClyde->setCanMove(false);
}


void GameManager::makeMainMenu(WPtr<Transform> canvasT)
{
  auto& framework = PongFramework::instance();
  auto& fontMan = gl::FontManager::instance();

  m_pMainMenu = getScene().lock()->addActor("mainMenu").lock();
  SPtr<Transform> mainMenuT = m_pMainMenu->getTransform().lock();
  mainMenuT->attachTo(canvasT);
  m_pMainMenu->setActive(false);

  SPtr<Actor> mainMenuTitle = getScene().lock()->addActor("title").lock();
  SPtr<Transform> mainMenuTitleT = mainMenuTitle->getTransform().lock();
  mainMenuTitleT->attachTo(mainMenuT);
  mainMenuTitleT->setLocalPosition({14.0f, 8.0f});
  mainMenuTitleT->setLocalScale({16.0f, 5.0f});
  SPtr<UIText> mainMenuTitleText = mainMenuTitle->addComponent<UIText>().lock();
  mainMenuTitleText->m_sText = "PacMan";
  mainMenuTitleText->m_iFontSize = 90u;
  mainMenuTitleText->m_pFont = fontMan.getFont("Crackman");
  SPtr<Render> mainMenuTitleRender = mainMenuTitle->addComponent<Render>().lock();
  mainMenuTitleRender->m_material.m_color = sf::Color::Yellow;
  //mainMenuTitle->addComponent<ShapeRect>();

  SPtr<Actor> mainMenuNewGameBtn = getScene().lock()->addActor("newGameBtn").lock();
  SPtr<Transform> mainMenuNewGameBtnT = mainMenuNewGameBtn->getTransform().lock();
  mainMenuNewGameBtnT->attachTo(mainMenuT);
  mainMenuNewGameBtnT->setLocalPosition({14.0f, 15.0f});
  mainMenuNewGameBtnT->setLocalScale({8.0f, 2.5f});
  mainMenuNewGameBtn->addComponent<ShapeRect>();
  SPtr<Render> mainMenuNewGameBtnRender = mainMenuNewGameBtn->addComponent<Render>().lock();
  mainMenuNewGameBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> mainMenuNewGameBtnComp = mainMenuNewGameBtn->addComponent<Button>().lock();
  mainMenuNewGameBtnComp->m_fpOnClickStops = [this] ()
  {
    std::cout << "New Game" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kStartGame);
  };

  SPtr<Actor> mainMenuNewGameBtnTxt = getScene().lock()->addActor("newGameBtnText").lock();
  SPtr<Transform> mainMenuNewGameBtnTxtT = mainMenuNewGameBtnTxt->getTransform().lock();
  mainMenuNewGameBtnTxtT->attachTo(mainMenuNewGameBtnT, false);
  mainMenuNewGameBtnTxtT->setScale({4.8f, 0.6f});
  SPtr<UIText> mainMenuNewGameBtnTxtText = mainMenuNewGameBtnTxt->addComponent<UIText>().lock();
  mainMenuNewGameBtnTxtText->m_sText = "New Game";
  mainMenuNewGameBtnTxtText->m_iFontSize = 15u;
  mainMenuNewGameBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> mainMenuNewGameBtnTxtRender = mainMenuNewGameBtnTxt->addComponent<Render>().lock();
  mainMenuNewGameBtnTxtRender->m_material.m_color = sf::Color::Black;
  //mainMenuNewGameBtnTxt->addComponent<ShapeRect>();

  SPtr<Actor> mainMenuLoadGameBtn = getScene().lock()->addActor("loadGameBtn").lock();
  SPtr<Transform> mainMenuLoadGameBtnT = mainMenuLoadGameBtn->getTransform().lock();
  mainMenuLoadGameBtnT->attachTo(mainMenuT);
  mainMenuLoadGameBtnT->setLocalPosition({14.0f, 20.0f});
  mainMenuLoadGameBtnT->setLocalScale({8.0f, 2.5f});
  mainMenuLoadGameBtn->addComponent<ShapeRect>();
  SPtr<Render> mainMenuLoadGameBtnRender = mainMenuLoadGameBtn->addComponent<Render>().lock();
  mainMenuLoadGameBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> mainMenuLoadGameBtnComp = mainMenuLoadGameBtn->addComponent<Button>().lock();
  mainMenuLoadGameBtnComp->m_fpOnClickStops = [] ()
  {
    std::cout << "Load Game" << std::endl;
  };

  SPtr<Actor> mainMenuLoadGameBtnTxt = getScene().lock()->addActor("loadGameBtnText").lock();
  SPtr<Transform> mainMenuLoadGameBtnTxtT = mainMenuLoadGameBtnTxt->getTransform().lock();
  mainMenuLoadGameBtnTxtT->attachTo(mainMenuLoadGameBtnT, false);
  mainMenuLoadGameBtnTxtT->setScale({5.5f, 0.6f});
  SPtr<UIText> mainMenuLoadGameBtnTxtText = mainMenuLoadGameBtnTxt->addComponent<UIText>().lock();
  mainMenuLoadGameBtnTxtText->m_sText = "Load Game";
  mainMenuLoadGameBtnTxtText->m_iFontSize = 15u;
  mainMenuLoadGameBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> mainMenuLoadGameBtnTxtRender = mainMenuLoadGameBtnTxt->addComponent<Render>().lock();
  mainMenuLoadGameBtnTxtRender->m_material.m_color = sf::Color::Black;
  //mainMenuLoadGameBtnTxt->addComponent<ShapeRect>();

  SPtr<Actor> mainMenuExitBtn = getScene().lock()->addActor("exitBtn").lock();
  SPtr<Transform> mainMenuExitBtnT = mainMenuExitBtn->getTransform().lock();
  mainMenuExitBtnT->attachTo(mainMenuT);
  mainMenuExitBtnT->setLocalPosition({14.0f, 25.0f});
  mainMenuExitBtnT->setLocalScale({8.0f, 2.5f});
  mainMenuExitBtn->addComponent<ShapeRect>();
  SPtr<Render> mainMenuExitBtnRender = mainMenuExitBtn->addComponent<Render>().lock();
  mainMenuExitBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> mainMenuExitBtnComp = mainMenuExitBtn->addComponent<Button>().lock();
  mainMenuExitBtnComp->m_fpOnClickStops = [&framework] ()
  {
    std::cout << "Exit" << std::endl;
    framework.close();
  };

  SPtr<Actor> mainMenuExitBtnTxt = getScene().lock()->addActor("exitBtnText").lock();
  SPtr<Transform> mainMenuExitBtnTxtT = mainMenuExitBtnTxt->getTransform().lock();
  mainMenuExitBtnTxtT->attachTo(mainMenuExitBtnT, false);
  mainMenuExitBtnTxtT->setScale({2.4f, 0.6f});
  SPtr<UIText> mainMenuExitBtnTxtText = mainMenuExitBtnTxt->addComponent<UIText>().lock();
  mainMenuExitBtnTxtText->m_sText = "Exit";
  mainMenuExitBtnTxtText->m_iFontSize = 15u;
  mainMenuExitBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> mainMenuExitBtnTxtRender = mainMenuExitBtnTxt->addComponent<Render>().lock();
  mainMenuExitBtnTxtRender->m_material.m_color = sf::Color::Black;
  //mainMenuExitBtnTxt->addComponent<ShapeRect>();
}
void GameManager::makePauseMenu(WPtr<Transform> canvasT)
{
  auto& framework = PongFramework::instance();
  auto& fontMan = gl::FontManager::instance();
  
  m_pPauseMenu = getScene().lock()->addActor("pauseMenu").lock();
  SPtr<Transform> pauseMenuT = m_pPauseMenu->getTransform().lock();
  pauseMenuT->attachTo(canvasT);
  // Esto cubre toda la pantalla. Eventualmente esto lo tiene que hacer el
  // transform de un obejto dentro del canvas, pero no hay tiempo para eso ahora.
  pauseMenuT->setScale({28.0f, 37.0f});
  pauseMenuT->setPosition({14.0f, 18.5f});
  m_pPauseMenu->addComponent<ShapeRect>();
  SPtr<Render> pauseMenuRender = m_pPauseMenu->addComponent<Render>().lock();
  pauseMenuRender->m_material.m_color = sf::Color(0, 0, 0, 100);
  m_pPauseMenu->setActive(false);

  SPtr<Actor> pauseMenuTitle = getScene().lock()->addActor("title").lock();
  SPtr<Transform> pauseMenuTitleT = pauseMenuTitle->getTransform().lock();
  pauseMenuTitleT->attachTo(pauseMenuT);
  pauseMenuTitleT->setPosition({14.0f, 8.0f});
  pauseMenuTitleT->setScale({13.6f, 1.25f});
  SPtr<UIText> pauseMenuTitleText = pauseMenuTitle->addComponent<UIText>().lock();
  pauseMenuTitleText->m_sText = "Game Paused";
  pauseMenuTitleText->m_iFontSize = 30u;
  pauseMenuTitleText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> pauseMenuTitleRender = pauseMenuTitle->addComponent<Render>().lock();
  pauseMenuTitleRender->m_material.m_color = sf::Color::Yellow;
  //pauseMenuTitle->addComponent<ShapeRect>();

  SPtr<Actor> pauseMenuResumeBtn = getScene().lock()->addActor("resumeBtn").lock();
  SPtr<Transform> pauseMenuResumeBtnT = pauseMenuResumeBtn->getTransform().lock();
  pauseMenuResumeBtnT->attachTo(pauseMenuT);
  pauseMenuResumeBtnT->setPosition({14.0f, 15.0f});
  pauseMenuResumeBtnT->setScale({8.0f, 2.5f});
  pauseMenuResumeBtn->addComponent<ShapeRect>();
  SPtr<Render> pauseMenuResumeBtnRender = pauseMenuResumeBtn->addComponent<Render>().lock();
  pauseMenuResumeBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> pauseMenuResumeBtnComp = pauseMenuResumeBtn->addComponent<Button>().lock();
  pauseMenuResumeBtnComp->m_fpOnClickStops = [this] ()
  {
    std::cout << "Resume Game" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kResumeGame);
  };

  SPtr<Actor> pauseMenuResumeBtnTxt = getScene().lock()->addActor("resumeBtnText").lock();
  SPtr<Transform> pauseMenuResumeBtnTxtT = pauseMenuResumeBtnTxt->getTransform().lock();
  pauseMenuResumeBtnTxtT->attachTo(pauseMenuResumeBtnT, false);
  pauseMenuResumeBtnTxtT->setScale({3.7f, 0.6f});
  SPtr<UIText> pauseMenuResumeBtnTxtText = pauseMenuResumeBtnTxt->addComponent<UIText>().lock();
  pauseMenuResumeBtnTxtText->m_sText = "Resume";
  pauseMenuResumeBtnTxtText->m_iFontSize = 15u;
  pauseMenuResumeBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> pauseMenuResumeBtnTxtRender = pauseMenuResumeBtnTxt->addComponent<Render>().lock();
  pauseMenuResumeBtnTxtRender->m_material.m_color = sf::Color::Black;
  //pauseMenuResumeBtnTxt->addComponent<ShapeRect>();

  SPtr<Actor> pauseMenuSaveGameBtn = getScene().lock()->addActor("saveGameBtn").lock();
  SPtr<Transform> pauseMenuSaveGameBtnT = pauseMenuSaveGameBtn->getTransform().lock();
  pauseMenuSaveGameBtnT->attachTo(pauseMenuT);
  pauseMenuSaveGameBtnT->setPosition({14.0f, 20.0f});
  pauseMenuSaveGameBtnT->setScale({8.0f, 2.5f});
  pauseMenuSaveGameBtn->addComponent<ShapeRect>();
  SPtr<Render> pauseMenuSaveGameBtnRender = pauseMenuSaveGameBtn->addComponent<Render>().lock();
  pauseMenuSaveGameBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> pauseMenuSaveGameBtnComp = pauseMenuSaveGameBtn->addComponent<Button>().lock();
  pauseMenuSaveGameBtnComp->m_fpOnClickStops = [this] ()
  {
    saveGame();
  };

  SPtr<Actor> pauseMenuSaveGameBtnTxt = getScene().lock()->addActor("saveGameBtnText").lock();
  SPtr<Transform> pauseMenuSaveGameBtnTxtT = pauseMenuSaveGameBtnTxt->getTransform().lock();
  pauseMenuSaveGameBtnTxtT->attachTo(pauseMenuSaveGameBtnT, false);
  pauseMenuSaveGameBtnTxtT->setScale({5.5f, 0.6f});
  SPtr<UIText> pauseMenuSaveGameBtnTxtText = pauseMenuSaveGameBtnTxt->addComponent<UIText>().lock();
  pauseMenuSaveGameBtnTxtText->m_sText = "Save Game";
  pauseMenuSaveGameBtnTxtText->m_iFontSize = 15u;
  pauseMenuSaveGameBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> pauseMenuSaveGameBtnTxtRender = pauseMenuSaveGameBtnTxt->addComponent<Render>().lock();
  pauseMenuSaveGameBtnTxtRender->m_material.m_color = sf::Color::Black;
  //pauseMenuSaveGameBtnTxt->addComponent<ShapeRect>();

  SPtr<Actor> pauseMenuMainMenuBtn = getScene().lock()->addActor("mainMenuBtn").lock();
  SPtr<Transform> pauseMenuMainMenuBtnT = pauseMenuMainMenuBtn->getTransform().lock();
  pauseMenuMainMenuBtnT->attachTo(pauseMenuT);
  pauseMenuMainMenuBtnT->setPosition({14.0f, 25.0f});
  pauseMenuMainMenuBtnT->setScale({8.0f, 2.5f});
  pauseMenuMainMenuBtn->addComponent<ShapeRect>();
  SPtr<Render> pauseMenuMainMenuBtnRender = pauseMenuMainMenuBtn->addComponent<Render>().lock();
  pauseMenuMainMenuBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> pauseMenuMainMenuBtnComp = pauseMenuMainMenuBtn->addComponent<Button>().lock();
  pauseMenuMainMenuBtnComp->m_fpOnClickStops = [this] ()
  {
    std::cout << "Main Menu" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kToMainMenu);
  };

  SPtr<Actor> pauseMenuMainMenuBtnTxt = getScene().lock()->addActor("mainMenuBtnText").lock();
  SPtr<Transform> pauseMenuMainMenuBtnTxtT = pauseMenuMainMenuBtnTxt->getTransform().lock();
  pauseMenuMainMenuBtnTxtT->attachTo(pauseMenuMainMenuBtnT, false);
  pauseMenuMainMenuBtnTxtT->setScale({5.5f, 0.6f});
  SPtr<UIText> pauseMenuMainMenuBtnTxtText = pauseMenuMainMenuBtnTxt->addComponent<UIText>().lock();
  pauseMenuMainMenuBtnTxtText->m_sText = "Main Menu";
  pauseMenuMainMenuBtnTxtText->m_iFontSize = 15u;
  pauseMenuMainMenuBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> pauseMenuMainMenuBtnTxtRender = pauseMenuMainMenuBtnTxt->addComponent<Render>().lock();
  pauseMenuMainMenuBtnTxtRender->m_material.m_color = sf::Color::Black;
  //pauseMenuMainMenuBtnTxt->addComponent<ShapeRect>();
}
void GameManager::makeGameMenu(WPtr<Transform> canvasT)
{
  auto& framework = PongFramework::instance();
  auto& fontMan = gl::FontManager::instance();

  m_pGameMenu = getScene().lock()->addActor("gameMenu").lock();
  SPtr<Transform> gameMenuT = m_pGameMenu->getTransform().lock();
  gameMenuT->attachTo(canvasT);
  m_pGameMenu->setActive(false);

  SPtr<Actor> gameMenuReadyText = getScene().lock()->addActor("readyText").lock();
  SPtr<Transform> gameMenuReadyTextT = gameMenuReadyText->getTransform().lock();
  gameMenuReadyTextT->attachTo(gameMenuT);
  gameMenuReadyTextT->setLocalPosition({14.0f, 21.5f});
  gameMenuReadyTextT->setLocalScale({6.0f, 1.1f});
  SPtr<UIText> gameMenuReadyTextText = gameMenuReadyText->addComponent<UIText>().lock();
  gameMenuReadyTextText->m_sText = "Ready!";
  gameMenuReadyTextText->m_iFontSize = 25u;
  gameMenuReadyTextText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> gameMenuReadyTextRender = gameMenuReadyText->addComponent<Render>().lock();
  gameMenuReadyTextRender->m_material.m_color = sf::Color::Yellow;
  //gameMenuReadyText->addComponent<ShapeRect>();
}
void GameManager::makeGameOverMenu(WPtr<Transform> canvasT)
{
  auto& framework = PongFramework::instance();
  auto& fontMan = gl::FontManager::instance();

  m_pGameOverMenu = getScene().lock()->addActor("gameOverMenu").lock();
  SPtr<Transform> gameOverMenuT = m_pGameOverMenu->getTransform().lock();
  gameOverMenuT->attachTo(canvasT);
  // Esto cubre toda la pantalla. Eventualmente esto lo tiene que hacer el
  // transform de un obejto dentro del canvas, pero no hay tiempo para eso ahora.
  gameOverMenuT->setScale({28.0f, 37.0f});
  gameOverMenuT->setPosition({14.0f, 18.5f});
  m_pGameOverMenu->addComponent<ShapeRect>();
  SPtr<Render> gameOverMenuRender = m_pGameOverMenu->addComponent<Render>().lock();
  gameOverMenuRender->m_material.m_color = sf::Color(100, 0, 0, 100);
  m_pGameOverMenu->setActive(false);

  SPtr<Actor> gameOverMenuTitle = getScene().lock()->addActor("title").lock();
  SPtr<Transform> gameOverMenuTitleT = gameOverMenuTitle->getTransform().lock();
  gameOverMenuTitleT->attachTo(gameOverMenuT);
  gameOverMenuTitleT->setPosition({14.0f, 8.0f});
  gameOverMenuTitleT->setScale({11.1f, 1.1f});
  SPtr<UIText> gameOverMenuTitleText = gameOverMenuTitle->addComponent<UIText>().lock();
  gameOverMenuTitleText->m_sText = "Game Over";
  gameOverMenuTitleText->m_iFontSize = 30u;
  gameOverMenuTitleText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> gameOverMenuTitleRender = gameOverMenuTitle->addComponent<Render>().lock();
  gameOverMenuTitleRender->m_material.m_color = sf::Color::Yellow;
  //gameOverMenuTitle->addComponent<ShapeRect>();

  SPtr<Actor> gameOverMenuRetryBtn = getScene().lock()->addActor("retryBtn").lock();
  SPtr<Transform> gameOverMenuRetryBtnT = gameOverMenuRetryBtn->getTransform().lock();
  gameOverMenuRetryBtnT->attachTo(gameOverMenuT);
  gameOverMenuRetryBtnT->setPosition({14.0f, 15.0f});
  gameOverMenuRetryBtnT->setScale({8.0f, 2.5f});
  gameOverMenuRetryBtn->addComponent<ShapeRect>();
  SPtr<Render> gameOverMenuRetryBtnRender = gameOverMenuRetryBtn->addComponent<Render>().lock();
  gameOverMenuRetryBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> gameOverMenuRetryBtnComp = gameOverMenuRetryBtn->addComponent<Button>().lock();
  gameOverMenuRetryBtnComp->m_fpOnClickStops = [this] ()
  {
    std::cout << "Retry Game" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kRestartGame);
  };

  SPtr<Actor> gameOverMenuRetryBtnTxt = getScene().lock()->addActor("retryBtnText").lock();
  SPtr<Transform> gameOverMenuRetryBtnTxtT = gameOverMenuRetryBtnTxt->getTransform().lock();
  gameOverMenuRetryBtnTxtT->attachTo(gameOverMenuRetryBtnT, false);
  gameOverMenuRetryBtnTxtT->setScale({3.2f, 0.6f});
  SPtr<UIText> gameOverMenuRetryBtnTxtText = gameOverMenuRetryBtnTxt->addComponent<UIText>().lock();
  gameOverMenuRetryBtnTxtText->m_sText = "Retry";
  gameOverMenuRetryBtnTxtText->m_iFontSize = 15u;
  gameOverMenuRetryBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> gameOverMenuRetryBtnTxtRender = gameOverMenuRetryBtnTxt->addComponent<Render>().lock();
  gameOverMenuRetryBtnTxtRender->m_material.m_color = sf::Color::Black;
  //gameOverMenuRetryBtnTxt->addComponent<ShapeRect>();

  SPtr<Actor> gameOverMenuMainMenuBtn = getScene().lock()->addActor("mainMenuBtn").lock();
  SPtr<Transform> gameOverMenuMainMenuBtnT = gameOverMenuMainMenuBtn->getTransform().lock();
  gameOverMenuMainMenuBtnT->attachTo(gameOverMenuT);
  gameOverMenuMainMenuBtnT->setPosition({14.0f, 20.0f});
  gameOverMenuMainMenuBtnT->setScale({8.0f, 2.5f});
  gameOverMenuMainMenuBtn->addComponent<ShapeRect>();
  SPtr<Render> gameOverMenuMainMenuBtnRender = gameOverMenuMainMenuBtn->addComponent<Render>().lock();
  gameOverMenuMainMenuBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> gameOverMenuMainMenuBtnComp = gameOverMenuMainMenuBtn->addComponent<Button>().lock();
  gameOverMenuMainMenuBtnComp->m_fpOnClickStops = [this] ()
  {
    std::cout << "Main Menu" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kToMainMenu);
  };

  SPtr<Actor> gameOverMenuMainMenuBtnTxt = getScene().lock()->addActor("mainMenuBtnText").lock();
  SPtr<Transform> gameOverMenuMainMenuBtnTxtT = gameOverMenuMainMenuBtnTxt->getTransform().lock();
  gameOverMenuMainMenuBtnTxtT->attachTo(gameOverMenuMainMenuBtnT, false);
  gameOverMenuMainMenuBtnTxtT->setScale({5.5f, 0.6f});
  SPtr<UIText> gameOverMenuMainMenuBtnTxtText = gameOverMenuMainMenuBtnTxt->addComponent<UIText>().lock();
  gameOverMenuMainMenuBtnTxtText->m_sText = "Main Menu";
  gameOverMenuMainMenuBtnTxtText->m_iFontSize = 15u;
  gameOverMenuMainMenuBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> gameOverMenuMainMenuBtnTxtRender = gameOverMenuMainMenuBtnTxt->addComponent<Render>().lock();
  gameOverMenuMainMenuBtnTxtRender->m_material.m_color = sf::Color::Black;
  //gameOverMenuMainMenuBtnTxt->addComponent<ShapeRect>();
}
void GameManager::makeGameWonMenu(WPtr<Transform> canvasT)
{
  auto& framework = PongFramework::instance();
  auto& fontMan = gl::FontManager::instance();

  m_pGameWonMenu = getScene().lock()->addActor("gameWonMenu").lock();
  SPtr<Transform> gameWonMenuT = m_pGameWonMenu->getTransform().lock();
  gameWonMenuT->attachTo(canvasT);
  // Esto cubre toda la pantalla. Eventualmente esto lo tiene que hacer el
  // transform de un obejto dentro del canvas, pero no hay tiempo para eso ahora.
  gameWonMenuT->setScale({28.0f, 37.0f});
  gameWonMenuT->setPosition({14.0f, 18.5f});
  m_pGameWonMenu->addComponent<ShapeRect>();
  SPtr<Render> gameWonMenuRender = m_pGameWonMenu->addComponent<Render>().lock();
  gameWonMenuRender->m_material.m_color = sf::Color(50, 100, 0, 100);
  m_pGameWonMenu->setActive(false);

  SPtr<Actor> gameWonMenuTitle = getScene().lock()->addActor("title").lock();
  SPtr<Transform> gameWonMenuTitleT = gameWonMenuTitle->getTransform().lock();
  gameWonMenuTitleT->attachTo(gameWonMenuT);
  gameWonMenuTitleT->setPosition({14.0f, 8.0f});
  gameWonMenuTitleT->setScale({11.1f, 1.1f});
  SPtr<UIText> gameWonMenuTitleText = gameWonMenuTitle->addComponent<UIText>().lock();
  gameWonMenuTitleText->m_sText = "Game Won!";
  gameWonMenuTitleText->m_iFontSize = 30u;
  gameWonMenuTitleText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> gameWonMenuTitleRender = gameWonMenuTitle->addComponent<Render>().lock();
  gameWonMenuTitleRender->m_material.m_color = sf::Color::Yellow;
  //gameWonMenuTitle->addComponent<ShapeRect>();

  SPtr<Actor> gameWonMenuRetryBtn = getScene().lock()->addActor("retryBtn").lock();
  SPtr<Transform> gameWonMenuRetryBtnT = gameWonMenuRetryBtn->getTransform().lock();
  gameWonMenuRetryBtnT->attachTo(gameWonMenuT);
  gameWonMenuRetryBtnT->setPosition({14.0f, 15.0f});
  gameWonMenuRetryBtnT->setScale({8.0f, 2.5f});
  gameWonMenuRetryBtn->addComponent<ShapeRect>();
  SPtr<Render> gameWonMenuRetryBtnRender = gameWonMenuRetryBtn->addComponent<Render>().lock();
  gameWonMenuRetryBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> gameWonMenuRetryBtnComp = gameWonMenuRetryBtn->addComponent<Button>().lock();
  gameWonMenuRetryBtnComp->m_fpOnClickStops = [this] ()
  {
    std::cout << "Retry Game" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kRestartGame);
  };

  SPtr<Actor> gameWonMenuRetryBtnTxt = getScene().lock()->addActor("retryBtnText").lock();
  SPtr<Transform> gameWonMenuRetryBtnTxtT = gameWonMenuRetryBtnTxt->getTransform().lock();
  gameWonMenuRetryBtnTxtT->attachTo(gameWonMenuRetryBtnT, false);
  gameWonMenuRetryBtnTxtT->setScale({3.2f, 0.6f});
  SPtr<UIText> gameWonMenuRetryBtnTxtText = gameWonMenuRetryBtnTxt->addComponent<UIText>().lock();
  gameWonMenuRetryBtnTxtText->m_sText = "Retry";
  gameWonMenuRetryBtnTxtText->m_iFontSize = 15u;
  gameWonMenuRetryBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> gameWonMenuRetryBtnTxtRender = gameWonMenuRetryBtnTxt->addComponent<Render>().lock();
  gameWonMenuRetryBtnTxtRender->m_material.m_color = sf::Color::Black;
  //gameWonMenuRetryBtnTxt->addComponent<ShapeRect>();

  SPtr<Actor> gameWonMenuMainMenuBtn = getScene().lock()->addActor("mainMenuBtn").lock();
  SPtr<Transform> gameWonMenuMainMenuBtnT = gameWonMenuMainMenuBtn->getTransform().lock();
  gameWonMenuMainMenuBtnT->attachTo(gameWonMenuT);
  gameWonMenuMainMenuBtnT->setPosition({14.0f, 20.0f});
  gameWonMenuMainMenuBtnT->setScale({8.0f, 2.5f});
  gameWonMenuMainMenuBtn->addComponent<ShapeRect>();
  SPtr<Render> gameWonMenuMainMenuBtnRender = gameWonMenuMainMenuBtn->addComponent<Render>().lock();
  gameWonMenuMainMenuBtnRender->m_material.m_color = sf::Color::White;
  SPtr<Button> gameWonMenuMainMenuBtnComp = gameWonMenuMainMenuBtn->addComponent<Button>().lock();
  gameWonMenuMainMenuBtnComp->m_fpOnClickStops = [this] ()
  {
    std::cout << "Main Menu" << std::endl;
    m_pStateMachine->externalInput(eEXTERNAL_INPUT::kToMainMenu);
  };

  SPtr<Actor> gameWonMenuMainMenuBtnTxt = getScene().lock()->addActor("mainMenuBtnText").lock();
  SPtr<Transform> gameWonMenuMainMenuBtnTxtT = gameWonMenuMainMenuBtnTxt->getTransform().lock();
  gameWonMenuMainMenuBtnTxtT->attachTo(gameWonMenuMainMenuBtnT, false);
  gameWonMenuMainMenuBtnTxtT->setScale({5.5f, 0.6f});
  SPtr<UIText> gameWonMenuMainMenuBtnTxtText = gameWonMenuMainMenuBtnTxt->addComponent<UIText>().lock();
  gameWonMenuMainMenuBtnTxtText->m_sText = "Main Menu";
  gameWonMenuMainMenuBtnTxtText->m_iFontSize = 15u;
  gameWonMenuMainMenuBtnTxtText->m_pFont = fontMan.getFont("PressStart2P");
  SPtr<Render> gameWonMenuMainMenuBtnTxtRender = gameWonMenuMainMenuBtnTxt->addComponent<Render>().lock();
  gameWonMenuMainMenuBtnTxtRender->m_material.m_color = sf::Color::Black;
  //gameWonMenuMainMenuBtnTxt->addComponent<ShapeRect>();
}