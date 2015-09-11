package buffer.policy;

import buffer.BufferFrame;
import disk.Page;
import disk.PageId;
import global.TestUtils;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.List;

import static org.junit.Assert.*;

public class GeneralisedClockReplacementPolicyTest {

    private ReplacementPolicy mReplacementPolicy;
    private List<BufferFrame> mPool;
    private BufferFrame A, B, C, D, E;

    @Before
    public void setUp() throws Exception {
        mReplacementPolicy = new GeneralisedClockReplacementPolicy(4);
        mPool = TestUtils.generateBufferFrameList(5);
        A = mPool.get(0);
        B = mPool.get(1);
        C = mPool.get(2);
        D = mPool.get(3);
        E = mPool.get(4);
    }

    @Test(timeout = 1000)
    public void testChooseWhenOneFrameUnused() throws Exception {
        TestUtils.notifyMany(mReplacementPolicy, mPool, 1, 1, 0, 1, 1);
        // Test 4/5 used, but one never used
        assertEquals(C, mReplacementPolicy.choose(mPool));
    }

    @Test(timeout = 1000)
    public void testChooseMiddleFrame() throws Exception {
        TestUtils.notifyMany(mReplacementPolicy, mPool, 4, 3, 4, 4, 4);
        // Test 4/5 used, but one never used
        assertEquals(B, mReplacementPolicy.choose(mPool));
    }

//    @Test
//    public void testChooseCountdownWithPin() throws Exception {
//        TestUtils.notifyMany(mReplacementPolicy, mPool, 4, 3, 2, 1, 1);
//        // Test 4/5 used, but one never used
//        assertEquals(D, mReplacementPolicy.choose(mPool));
//        D.pin();
//        assertEquals(E, mReplacementPolicy.choose(mPool));
//        E.pin();
//        assertEquals(C, mReplacementPolicy.choose(mPool));
//        C.pin();
//        assertEquals(B, mReplacementPolicy.choose(mPool));
//        B.pin();
//        assertEquals(A, mReplacementPolicy.choose(mPool));
//    }
//
//    @Test
//    public void testIncrementsWhenPinned() throws Exception {
//        for(BufferFrame frame : mPool) {
//            frame.pin();
//        }
//        TestUtils.notifyMany(mReplacementPolicy, mPool, 4, 4, 0, 4, 4);
//        for(BufferFrame frame : mPool) {
//            frame.unpin();
//        }
//        assertEquals(C, mReplacementPolicy.choose(mPool));
//    }

    @Test(timeout = 1000)
    public void testClockLimit() throws Exception {
        TestUtils.notifyMany(mReplacementPolicy, mPool, 10, 4, 4, 4, 4);
        assertEquals(A, mReplacementPolicy.choose(mPool));

        TestUtils.notifyMany(mReplacementPolicy, mPool, 4, 4, 4, 4, 10);
        mReplacementPolicy.choose(mPool);
        TestUtils.notifyMany(mReplacementPolicy, mPool, 4, 4, 4, 4, 0);
        assertEquals(E, mReplacementPolicy.choose(mPool));
    }

    @Test(timeout = 1000)
    public void testMoveToNextFrameAfterChosen() throws Exception {
        TestUtils.notifyMany(mReplacementPolicy, mPool, 0, 0, 0, 0, 0);
        assertEquals(A, mReplacementPolicy.choose(mPool));
        assertEquals(B, mReplacementPolicy.choose(mPool));
        assertEquals(C, mReplacementPolicy.choose(mPool));
    }

    @Test(timeout = 1000)
    public void testChooseRecentlyReplacedFrame() throws Exception {
        TestUtils.notifyMany(mReplacementPolicy, mPool, 1, 1, 1, 1, 1);
        PageId replaceId = new PageId(10);
        Page replacePage = new Page();
        C.setPage(replaceId, replacePage);
        assertEquals(C, mReplacementPolicy.choose(mPool));
    }

    @After
    public void testNoChangeToList() {
        assertEquals(5, mPool.size());
    }

//    @Test(expected= BufferFrame.AllBufferFramesPinnedException.class)
//    public void testChooseAllFramesPinned() {
//        A.pin();
//        B.pin();
//        C.pin();
//        D.pin();
//        E.pin();
//        mReplacementPolicy.choose(mPool);
//    }
}