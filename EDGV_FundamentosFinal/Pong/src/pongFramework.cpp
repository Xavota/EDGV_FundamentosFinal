#include "pongFramework.h"

#include <platform/iofile.h>
#include <platform/dllDynamics.h>

#include <scene/sceneManager.h>
#include <scene/scene.h>
#include <scene/actor.h>

#include <tools/textureManager.h>
#include <tools/fontManager.h>

#include "scripts/gameManager.h"

#include "scripts/mapManager.h"


void PongFramework::onInit()
{
  //std::cout << "OnInit" << std::endl;

  loadMods();

  /*if (!MapManager::isStarted()) {
    MapManager::startUp();
  }*/

  auto& textureMan = gl::TextureManager::instance();
  auto& fontMan = gl::FontManager::instance();

  Vector<Path> paths =
   File::getChildPaths(L"../Pong/resources/sprites/", true);
   //File::getChildPaths(L"C:/Develop/Especialidad/Proyectos/EDGV_FundamentosFinal/EDGV_FundamentosFinal/Pong/resources/sprites/", true);
  std::cout << "Textures count: " << paths.size() << std::endl;
  for (const auto& p : paths) {
    String textureName =
     std::filesystem::relative(p, "../Pong/resources/sprites/").generic_string();
    std::replace(textureName.begin(), textureName.end(), '/', '_');
    textureName = textureName.substr(0, textureName.size() - 4);

    std::cout << "Loading texture: " << textureName << std::endl;

    textureMan.addTexture(textureName, p.generic_string(), sf::Color::Black);
  }

  paths.clear();
  paths = File::getChildPaths(L"../Pong/resources/fonts/", true);
   //File::getChildPaths(L"C:/Develop/Especialidad/Proyectos/EDGV_FundamentosFinal/EDGV_FundamentosFinal/Pong/resources/sprites/", true);
  std::cout << "Fonts count: " << paths.size() << std::endl;
  for (const auto& p : paths) {
    String fontName =
     std::filesystem::relative(p, "../Pong/resources/fonts/").generic_string();
    std::replace(fontName.begin(), fontName.end(), '/', '_');
    fontName = fontName.substr(0, fontName.size() - 4);

    std::cout << "Loading font: " << fontName << std::endl;

    fontMan.addFont(fontName, p.generic_string());
  }


  SceneManager::instance().m_bDebug = false;
  SPtr<Scene> testScene = SceneManager::instance().addScene("TestScene").lock();

  SPtr<Actor> gameManager = testScene->addActor("GameManager").lock();
  gameManager->addComponent<GameManager>();


  /*SPtr<Actor> testActor = testScene->addActor("TestActor").lock();
  testActor->addComponent<ShapeCircle>();
  //testActor->addComponent<ShapeRect>();
  SPtr<Render> testActorRender = testActor->addComponent<Render>().lock();
  testActorRender->m_material.m_color = sf::Color::Red;
  SPtr<Transform> testActorTransform = testActor->getTransform().lock();
  testActorTransform->setScale({5.0f, 3.0f});
  testActorTransform->setPosition({15.0f, 8.0f});
  SPtr<RectCollider> testActorColl = testActor->addComponent<RectCollider>().lock();
  //testActor3Coll->offset = {2.0f, 3.0f};
  testActorColl->scale = {1.0f, 1.0f};
  testActorColl->render = true;
  
  SPtr<Actor> testActor2 = testScene->addActor("TestActor2").lock();
  testActor2->addComponent<ShapeCircle>();
  SPtr<Render> testActor2Render = testActor2->addComponent<Render>().lock();
  testActor2Render->m_material.m_color = sf::Color::Blue;
  testActor2Render->m_iRenderIndex = 1;
  SPtr<Transform> testActor2Transform = testActor2->getTransform().lock();
  //testActor2Transform->setScale({0.4f, 0.4f});
  testActor2Transform->setPosition({8.0f, 8.0f});
  SPtr<CircleCollider> testActor2Coll = testActor2->addComponent<CircleCollider>().lock();
  //testActor2Coll->offset = {5.0f, 2.0f};
  testActor2Coll->scaleRadius = 3.0f;
  testActor2Coll->render = true;
  testActor2->addComponent<MovementScript>();
  
  SPtr<Actor> testActor3 = testScene->addActor("TestActor3").lock();
  testActor3->addComponent<ShapeRect>();
  SPtr<Render> testActor3Render = testActor3->addComponent<Render>().lock();
  testActor3Render->m_material.m_color = sf::Color::Green;
  testActor3Render->m_iRenderIndex = 2;
  SPtr<Transform> testActor3Transform = testActor3->getTransform().lock();
  testActor3Transform->setScale({2.0f, 2.0f});
  testActor3Transform->setPosition({8.0f, 8.0f});
  SPtr<RectCollider> testActor3Coll = testActor3->addComponent<RectCollider>().lock();
  //testActor3Coll->offset = {2.0f, 3.0f};
  testActor3Coll->scale = {4.0f, 4.0f};
  testActor3Coll->render = true;
  testActor3Coll->setActive(false);*/

  SceneManager::instance().openScene("TestScene");
}
void PongFramework::onUpdate()
{

}
void PongFramework::onRender()
{

}
void PongFramework::onDestroy()
{

}

#if PLATFORM == PLATFORM_WIN32
  String libExtension = ".dll";
#else
  String libExtension = ".so";
#endif

void PongFramework::loadMods()
{
  Vector<Path> paths =
   //File::getChildPaths(L"../mods/", true);
   File::getChildPaths(L"./lib/Debug/", true);
  std::cout << "Mods count: " << paths.size() << std::endl;
  for (const auto& p : paths) {
    String fileExtension = p.extension().generic_string();
    //std::cout << "File extension: '" << fileExtension << "'" << std::endl;
    if (fileExtension != libExtension) continue;

    String modName =
     std::filesystem::relative(p, "../mods/").generic_string();
    std::replace(modName.begin(), modName.end(), '/', '_');
    modName = modName.substr(0, modName.size() - 4);

    std::cout << "Loading mod: " << modName << std::endl;

    DLLDynamics modDLL(p.generic_wstring());
    FunctionPtr<void> initModFnc = modDLL.getFunction("initMod");
    if (initModFnc) {
      initModFnc();
    }
  }
}