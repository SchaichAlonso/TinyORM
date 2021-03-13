#ifndef UTILS_TYPE_H
#define UTILS_TYPE_H

#include <QRegularExpression>

#ifdef TINYORM_COMMON_NAMESPACE
namespace TINYORM_COMMON_NAMESPACE
{
#endif
namespace Orm::Utils::Type
{
    /*! Class name without a namespace and template parameters. */
    template<typename Type>
    inline QString classPureBasename()
    {
        QRegularExpression re(
                    QStringLiteral("(?:(?<=^struct )\\w+|(?<=^class )\\w+|(?<=::)\\w+)"
                                   "(?=<.*>| |$)"));

        const auto match = re.match(typeid (Type).name());

        // This should never happen, but who knows 🤔
        Q_ASSERT_X(match.hasMatch(),
                   "regex match", "Can not get the class base name in "
                                  "Utils::Type::classPureBasename().");

        return match.captured();
    }

    /*! Class name without a namespace and template parameters. */
    template<typename Type>
    inline QString classPureBasename(const Type &type)
    {
        /* If you want to obtain a name for the polymorphic type, take care to pass
            a glvalue as the 'type' argument, the 'this' pointer is a prvalue! */
        QRegularExpression re(
                    QStringLiteral("(?:(?<=^struct )\\w+|(?<=^class )\\w+|(?<=::)\\w+)"
                                   "(?=<.*>| |$)"));

        const auto match = re.match(typeid (type).name());

        // This should never happen, but who knows 🤔
        Q_ASSERT_X(match.hasMatch(),
                   "regex match", "Can not get the class base name in "
                                  "Utils::Type::classPureBasename().");

        return match.captured();
    }

} // namespace Orm::Utils::Type
#ifdef TINYORM_COMMON_NAMESPACE
} // namespace TINYORM_COMMON_NAMESPACE
#endif

#endif // UTILS_TYPE_H
