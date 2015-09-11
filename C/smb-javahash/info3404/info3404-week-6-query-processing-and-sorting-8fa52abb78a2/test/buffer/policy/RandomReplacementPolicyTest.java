package buffer.policy;

import buffer.BufferFrame;
import buffer.policy.RandomReplacementPolicy;
import buffer.policy.ReplacementPolicy;
import global.TestUtils;
import org.junit.Before;
import org.junit.Test;

import java.util.List;

import static org.junit.Assert.*;

public class RandomReplacementPolicyTest {

    private ReplacementPolicy mReplacementPolicy;
    private List<BufferFrame> mPool;

    @Before
    public void setUp() throws Exception {
        mReplacementPolicy = new RandomReplacementPolicy();
        mPool = TestUtils.generateBufferFrameList(10);
    }

    @Test(timeout = 1000)
    public void testRandomChoose() throws Exception {
        boolean isRandomPolicy = false;
        for(int i = 0; i < 20; i++) {
            BufferFrame first = mReplacementPolicy.choose(mPool);
            BufferFrame second = mReplacementPolicy.choose(mPool);
            if(first != null && second != null && !first.equals(second)) {
                isRandomPolicy = true;
            }
        }
        assertTrue(isRandomPolicy);
    }
}