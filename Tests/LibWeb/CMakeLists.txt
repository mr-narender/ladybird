set(TEST_SOURCES
    TestCSSIDSpeed.cpp
    TestCSSInheritedProperty.cpp
    TestCSSPixels.cpp
    TestCSSSyntaxParser.cpp
    TestCSSTokenStream.cpp
    TestFetchInfrastructure.cpp
    TestFetchURL.cpp
    TestHTMLTokenizer.cpp
    TestMicrosyntax.cpp
    TestMimeSniff.cpp
    TestNumbers.cpp
    TestStrings.cpp
)

foreach(source IN LISTS TEST_SOURCES)
    ladybird_test("${source}" LibWeb LIBS LibWeb)
endforeach()

target_link_libraries(TestFetchURL PRIVATE LibURL)

if (ENABLE_SWIFT)
    find_package(SwiftTesting REQUIRED)

    add_executable(TestLibWebSwift
        TestLibWebSwiftBindings.swift
        TestHTMLTokenizerSwift.swift
    )

    # FIXME: Swift doesn't seem to like object libraries for @main
    target_sources(TestLibWebSwift PRIVATE ../Resources/SwiftTestMain.swift)

    set_target_properties(TestLibWebSwift PROPERTIES SUFFIX .swift-testing)
    target_link_libraries(TestLibWebSwift PRIVATE AK LibWeb LibGC SwiftTesting::SwiftTesting)
    add_test(NAME TestLibWebSwift COMMAND TestLibWebSwift)
endif()

add_subdirectory("test-web")
