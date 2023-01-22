#include <gtest/gtest.h>

#include <imk.h>

TEST(CSGNodeTests, create_empty_sane_defaults) {
    auto node = imk::csg::CSGNode::create_empty();
    EXPECT_EQ(node.type, imk::csg::CSGNode::Type::EMPTY);
    EXPECT_EQ(node.data(), nullptr);
    EXPECT_EQ(node.lhs().type, imk::csg::CSGNode::Type::EMPTY);
    EXPECT_EQ(node.rhs().type, imk::csg::CSGNode::Type::EMPTY);
}

TEST(CSGNodeTests, dfs_visitor) {
    using Type = imk::csg::CSGNode::Type;
    using State = imk::csg::CSGNode::VisitorIterator::VisitorState;

    auto root = imk::csg::CSGNode::create_empty();
    auto lhs = imk::csg::CSGNode::create(Type::Sphere, nullptr);
    auto rhs = imk::csg::CSGNode::create(Type::Plane, nullptr);

    root.lhs(std::move(lhs));
    root.rhs(std::move(rhs));

    auto it = root.begin();
    ASSERT_EQ(it->type, Type::EMPTY);
    ASSERT_EQ(it.state(), State::PreOrder);

    ASSERT_EQ((++it)->type, Type::Sphere);
    ASSERT_EQ(it.state(), State::PreOrder);

    ASSERT_EQ((++it)->type, Type::Sphere);
    ASSERT_EQ(it.state(), State::InOrder);

    ASSERT_EQ((++it)->type, Type::Sphere);
    ASSERT_EQ(it.state(), State::PostOrder);

    ASSERT_EQ((++it)->type, Type::EMPTY);
    ASSERT_EQ(it.state(), State::InOrder);

    ASSERT_EQ((++it)->type, Type::Plane);
    ASSERT_EQ(it.state(), State::PreOrder);

    ASSERT_EQ((++it)->type, Type::Plane);
    ASSERT_EQ(it.state(), State::InOrder);

    ASSERT_EQ((++it)->type, Type::Plane);
    ASSERT_EQ(it.state(), State::PostOrder);

    ASSERT_EQ((++it)->type, Type::EMPTY);
    ASSERT_EQ(it.state(), State::PostOrder);

    ASSERT_EQ(++it, root.end());
}