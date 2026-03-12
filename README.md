# minimal_daemon_cpp_project

Минимальный C++ проект с CMake, GoogleTest, LLVM-style .clang-format, pre-commit hook для clang-format и базовым clang-tidy.


В каталоге \tools:
- pre-commit (При наступлении события "commit" Git находит этот файл. Он содержит Git hook который запускает clang-format и clang-tidy для исходных файлов проекта)
- install-hooks.sh (Скрипт копирует шаблоны (pre-commit) в .git/hooks т.к. .git не отслеживается системой контроля версий. Использовать один раз при начале работы над проектом на машине)




# Настройка dev окружения
1) Установите clang-format и clang-tidy (и clangd при желании).
2) Выполните в корне репо:
   chmod +x tools/install-hooks.sh tools/pre-commit.sh
   tools/install-hooks.sh
3) Выполните out-of-source сборку с CMake:
   mkdir -p build && cd build
   cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
4) Теперь pre-commit будет форматировать staged файлы и запускать clang-tidy (если есть build/compile_commands.json).
