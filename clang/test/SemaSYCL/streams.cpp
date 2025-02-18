// RUN: %clang_cc1 -S -I %S/Inputs -fsycl -fsycl-is-device -triple spir64 -ast-dump %s | FileCheck %s

#include <sycl.hpp>

template <typename Name, typename Func>
__attribute__((sycl_kernel)) void kernel(const Func &kernelFunc) {
  kernelFunc();
}

using namespace cl::sycl;

handler H;

struct HasStreams {
  stream s1{0, 0, H};
  stream s_array[2] = {{0, 0, H}, {0, 0, H}};
};

struct HasArrayOfHasStreams {
  int i;
  HasStreams hs[2];
};

void use() {
  stream in_lambda{0, 0, H};
  stream in_lambda_array[2] = {{0, 0, H}, {0, 0, H}};
  stream in_lambda_mdarray[2][2] = {{{0, 0, H}, {0, 0, H}}, {{0, 0, H}, {0, 0, H}}};

  HasStreams Struct;
  HasArrayOfHasStreams haohs;
  HasArrayOfHasStreams haohs_array[2];

  kernel<class stream_test>([=]() {
    in_lambda.use();
    in_lambda_array[1].use();
    in_lambda_mdarray[1][1].use();

    Struct.s1.use();

    haohs.hs[0].s1.use();
    haohs_array[0].hs[0].s1.use();
  });
}

// Function Declaration
// CHECK: FunctionDecl {{.*}}stream_test{{.*}}

// Initializers:
// CHECK: InitListExpr {{.*}} '(lambda at
// 'in_lambda'
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar

// 'in_lambda_array'
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar

// 'in_lambda_mdarray'
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2][2]'
// sub-array 0
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// sub-array 1
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar

// HasStreams struct
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams'
// HasStreams::s1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams::s_array
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar

// HasArrayOfHasStreams
// CHECK-NEXT: InitListExpr {{.*}} 'HasArrayOfHasStreams'
// HasArrayOfHasStreams::i
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'int' <LValueToRValue>
// CHECK-NEXT: DeclRefExpr {{.*}} 'int' lvalue ParmVar
// HasArrayOfHasStreams::hs
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams [2]'
// HasStreams struct
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams'
// HasStreams::s1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams::s_array
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams struct
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams'
// HasStreams::s1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams::s_array
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar

// HasArrayOfHasStreams Array
// CHECK-NEXT: InitListExpr {{.*}} 'HasArrayOfHasStreams [2]'
// CHECK-NEXT: InitListExpr {{.*}} 'HasArrayOfHasStreams'
// HasArrayOfHasStreams::i
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'int' <LValueToRValue>
// CHECK-NEXT: DeclRefExpr {{.*}} 'int' lvalue ParmVar
// HasArrayOfHasStreams::hs
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams [2]'
// HasStreams struct
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams'
// HasStreams::s1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams::s_array
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams struct
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams'
// HasStreams::s1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams::s_array
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// CHECK-NEXT: InitListExpr {{.*}} 'HasArrayOfHasStreams'
// HasArrayOfHasStreams::i
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'int' <LValueToRValue>
// CHECK-NEXT: DeclRefExpr {{.*}} 'int' lvalue ParmVar
// HasArrayOfHasStreams::hs
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams [2]'
// HasStreams struct
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams'
// HasStreams::s1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams::s_array
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams struct
// CHECK-NEXT: InitListExpr {{.*}} 'HasStreams'
// HasStreams::s1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// HasStreams::s_array
// CHECK-NEXT: InitListExpr {{.*}} 'cl::sycl::stream [2]'
// element 0
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar
// element 1
// CHECK-NEXT: CXXConstructExpr {{.*}} 'cl::sycl::stream' 'void (const cl::sycl::stream &) noexcept'
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'const cl::sycl::stream' lvalue <NoOp>
// CHECK-NEXT: DeclRefExpr {{.*}} 'cl::sycl::stream' lvalue ParmVar

// Calls to Init, note that the accessor in the stream comes first, since the
// stream __init call depends on the accessor's call already having happened.
// in_lambda __init
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at

// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at

// _in_lambda_array
// element 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0

// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0

// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// _in_lambda_mdarray
// [0][0]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// [0][1]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// [1][0]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// [1][1]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// HasStreams
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// HasArrayOfHasStreams
// First element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// HasArrayOfHasStreams array
// First element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void (cl::sycl::accessor{{.*}})' lvalue .__init
// CHECK-NEXT: MemberExpr {{.*}} 'accessor<{{.*}}' lvalue .acc
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__init
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// Finalize
// in_lambda __finalize
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at

// _in_lambda_array
// element 0
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// _in_lambda_mdarray
// [0][0]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// [0][1]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// [1][0]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// [1][1]
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream [2]' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream (*)[2]' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2][2]' lvalue
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// HasStreams
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// HasArrayOfHasStreams
// First element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1

// HasArrayOfHasStreams array
// First element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// second element
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream' lvalue .s1
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// array:
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 0
// element 1
// CHECK: CXXMemberCallExpr {{.*}} 'void'
// CHECK-NEXT: MemberExpr {{.*}} 'void ()' lvalue .__finalize
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'cl::sycl::stream' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'cl::sycl::stream *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'cl::sycl::stream [2]' lvalue .s_array
// CHECK-NEXT: ArraySubscriptExpr {{.*}} 'HasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasStreams [2]' lvalue .hs
// CHECK-NEXT: ArraySubscriptExpr{{.*}} 'HasArrayOfHasStreams' lvalue
// CHECK-NEXT: ImplicitCastExpr {{.*}} 'HasArrayOfHasStreams *' <ArrayToPointerDecay>
// CHECK-NEXT: MemberExpr {{.*}} 'HasArrayOfHasStreams [2]' lvalue .
// CHECK-NEXT: DeclRefExpr {{.*}} '(lambda at
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
// CHECK-NEXT: IntegerLiteral {{.*}} '{{.*}}' 1
